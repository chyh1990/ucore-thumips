PROJ	:= 5
EMPTY	:=
SPACE	:= $(EMPTY) $(EMPTY)
SLASH	:= /

V       := @

#GCCPREFIX:=mips-sde-elf-
GCCPREFIX ?= /home/guest/cpu/build-gcc/mips_gcc/bin/mips-sde-elf-

# eliminate default suffix rules
.SUFFIXES: .c .S .h

# define compiler and flags

HOSTCC		:= gcc
HOSTCFLAGS	:= -g -Wall -O2

GDB		:= $(GCCPREFIX)gdb

THUMIPSCC		:= ./thumips-cc
CLANG := clang
CC :=$(GCCPREFIX)gcc
CFLAGS	:=  -fno-builtin -nostdlib  -nostdinc -g  -EL -G0 -fno-delayed-branch -Wa,-O0
CTYPE	:= c S

LD      := $(GCCPREFIX)ld
AS      := $(GCCPREFIX)as -EL -g -mips32
AR      := $(GCCPREFIX)ar
LDFLAGS	+= -nostdlib

OBJCOPY := $(GCCPREFIX)objcopy
OBJDUMP := $(GCCPREFIX)objdump

COPY	:= cp
MKDIR   := mkdir -p
MV		:= mv
RM		:= rm -f
AWK		:= awk
SED		:= sed
SH		:= sh
TR		:= tr
TOUCH	:= touch -c

TAR		:= tar
ZIP		:= gzip

OBJDIR	:= obj
BINDIR	:= bin
SRCDIR  := kern
DEPDIR  := dep


MODULES   := init libs debug driver trap mm sync process schedule syscall
SRC_DIR   := $(addprefix $(SRCDIR)/,$(MODULES))
BUILD_DIR := $(addprefix $(OBJDIR)/,$(MODULES))
DEP_DIR   := $(addprefix $(DEPDIR)/,$(MODULES))
VPATH     += $(SRC_DIR)

SRC       := $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.c))
OBJ       := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))
ASMSRC    := $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.S))
OBJ       += $(patsubst $(SRCDIR)/%.S, $(OBJDIR)/%.o, $(ASMSRC))
INCLUDES  := $(addprefix -I,$(SRC_DIR))
INCLUDES  += -I$(SRCDIR)/include

USER_APPLIST:= forktest yield hello faultreadkernel faultread badarg waitkill pgdir exit
USER_SRCDIR := user
USER_OBJDIR := $(OBJDIR)/$(USER_SRCDIR)
USER_LIB_OBJDIR := $(USER_OBJDIR)/libs
USER_INCLUDE := -I$(USER_SRCDIR)/libs

USER_APP_BINS:= $(addprefix $(USER_OBJDIR)/, $(USER_APPLIST))

USER_LIB_SRCDIR := $(USER_SRCDIR)/libs
USER_LIB_SRC := $(foreach sdir,$(USER_LIB_SRCDIR),$(wildcard $(sdir)/*.c))
USER_LIB_OBJ := $(patsubst $(USER_LIB_SRCDIR)/%.c, $(USER_LIB_OBJDIR)/%.o, $(USER_LIB_SRC))
USER_LIB_OBJ += $(USER_LIB_OBJDIR)/initcode.o
USER_LIB    := $(USER_OBJDIR)/libuser.a

BUILD_DIR   += $(USER_LIB_OBJDIR)
BUILD_DIR   += $(USER_OBJDIR)


DEPENDS := $(patsubst $(SRCDIR)/%.c, $(DEPDIR)/%.d, $(SRC))

MAKEDEPEND = $(CLANG) -M $(CFLAGS) $(INCLUDES) -o $(DEPDIR)/$*.d $<
#vpath %.c $(SRC_DIR)
#vpath %.S $(SRC_DIR)

.PHONY: all checkdirs clean 

all: checkdirs  obj/ucore-kernel-piggy

$(shell mkdir -p $(DEP_DIR))


obj/ucore-kernel:   $(OBJ) tools/kernel.ld
	@echo LINK $@
	$(LD) -nostdlib -n -G 0 -static -T tools/kernel.ld $(OBJ) -o $@

obj/ucore-kernel-piggy: $(BUILD_DIR)  $(OBJ) $(USER_APP_BINS) tools/kernel.ld
	@echo LINK $@
	$(LD) -nostdlib -n -G 0 -static -T tools/kernel.ld $(OBJ) \
					$(addsuffix .piggy.o, $(USER_APP_BINS)) -o $@

$(DEPDIR)/%.d: $(SRCDIR)/%.c
	@echo DEP $<
	@set -e; rm -f $@; \
		$(CC) -MM -MT "$(OBJDIR)/$*.o $@" $(CFLAGS) $(INCLUDES) $< > $@; 

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c -mips1 $(INCLUDES) $(CFLAGS)  $<  -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.S
	$(CC) -mips32 -c -D__ASSEMBLY__ $(INCLUDES) -g -EL -G0  $<  -o $@

checkdirs: $(BUILD_DIR) $(DEP_DIR)

$(BUILD_DIR):
	@mkdir -p $@

$(DEP_DIR):
	@mkdir -p $@

clean:
	-rm -rf $(BUILD_DIR)
	-rm -rf $(DEPDIR)


ifneq ($(MAKECMDGOALS),clean)
-include $(DEPENDS)
endif

#user lib

$(USER_LIB): $(BUILD_DIR) $(USER_LIB_OBJ)
	@echo "Building USERLIB"
	$(AR) rcs $@ $(USER_LIB_OBJ)

#user applications
    #$(CC) $(INCLUDES)  $$< -o $$@
	#$(OBJCOPY) -O elf32-tradlittlemips -I binary -B mips $$@  $$@.piggy.o
define make-user-app
$1: $(BUILD_DIR) $(addsuffix .o,$1) $(USER_LIB)
	@echo LINK $$@
	$(LD) -T $(USER_LIB_SRCDIR)/user.ld  $(addsuffix .o,$1) $(USER_LIB) -o $$@
	$(SED) 's/$$$$FILE/$(notdir $1)/g' tools/piggy.S.in > $(USER_OBJDIR)/piggy.S
	$(AS) $(USER_OBJDIR)/piggy.S -o $$@.piggy.o
endef

$(foreach bdir,$(USER_APP_BINS),$(eval $(call make-user-app,$(bdir))))

$(USER_OBJDIR)/%.o: $(USER_SRCDIR)/%.c
	$(CC) -c -mips1  $(USER_INCLUDE)  $(CFLAGS)  $<  -o $@

$(USER_OBJDIR)/%.o: $(USER_SRCDIR)/%.S
	$(CC) -mips32 -c -D__ASSEMBLY__ $(USER_INCLUDE) -g -EL -G0  $<  -o $@


