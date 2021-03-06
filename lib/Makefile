LIBS=-lm
MAJOR_VERSION=0
MINOR_VERSION=3
CSTD=-std=c99
CFLAGS=-Wall -Wextra -W -w -O0 $(CSTD) -DMAJOR_VERSION=$(MAJOR_VERSION) -DMINOR_VERSION=$(MINOR_VERSION) -fPIC -DPIC -DLINUX -D_XOPEN_SOURCE=700
DEBUG_CFLAGS=-Wall -Wextra -Wformat=0 -pedantic -fbounds-check -Wuninitialized -O0 -g3 $(CSTD) -DUSE_DEBUGGING -DMAJOR_VERSION=$(MAJOR_VERSION) -DMINOR_VERSION=$(MINOR_VERSION) -fPIC -DPIC -DLINUX -D_XOPEN_SOURCE=700
#-pedantic -fbounds-check -Wuninitialized -O0 -g3 -Wstrict-prototypes
LDFLAGS=$(LIBS)
DEBUG_LDFLAGS= $(LIBS)
CC=gcc
AR=ar -c
LD=ld

LIBDBUS=$(shell echo \\\#include \<dbus/dbus.h\>\\n int main\(int c,char**v\){} | gcc `pkg-config --libs dbus-1` `pkg-config --cflags dbus-1`  -xc -o a.out - 2>/dev/null ;echo $$? ;rm a.out 2>/dev/null )

ifeq ($(LIBDBUS), 0)
	LIBS+= `pkg-config --libs dbus-1` 
	CFLAGS+= `pkg-config --cflags dbus-1` -DUSE_DBUS
	DEBUG_CFLAGS+= `pkg-config --cflags dbus-1` -DUSE_DBUS
endif


RAZER_CHROMA_FILES=razer_chroma.c list.c razer_string.c
RAZER_CHROMA_INCLUDE_FILES=razer_chroma.h list.h razer_string.h
RAZER_CHROMA_OBJ=razer_chroma.o list.o razer_string.o
RAZER_CHROMA_DEBUG_OBJ=razer_chroma.do list.do razer_string.do
RAZER_CHROMA_CONTROLLER_OBJ=razer_chroma_controller.o list.o razer_string.o
RAZER_CHROMA_CONTROLLER_DEBUG_OBJ=razer_chroma_controller.do list.do razer_string.do

all: library library_debug

library_debug: razer_chroma_static_debug razer_chroma_shared_debug razer_chroma_controller_shared_debug razer_chroma_controller_static_debug
	@echo "::\033[32m COMPILING librazer_chroma.so [DEBUG]\033[0m"
	@echo "========================================"

library: razer_chroma_static razer_chroma_shared razer_chroma_controller_shared razer_chroma_controller_static
	@echo "::\033[32m COMPILING librazer_chroma.so\033[0m"
	@echo "================================"

razer_chroma_static: $(RAZER_CHROMA_OBJ)
	$(AR) -rs librazer_chroma.a $(RAZER_CHROMA_OBJ)

razer_chroma_static_debug: $(RAZER_CHROMA_DEBUG_OBJ)
	$(AR) -rs librazer_chroma.da $(RAZER_CHROMA_DEBUG_OBJ)

razer_chroma_shared: $(RAZER_CHROMA_OBJ)
	$(CC) -rdynamic -nostdlib -DCREATELIB -shared $(RAZER_CHROMA_OBJ) $(LDFLAGS) -o librazer_chroma.so
	@strip librazer_chroma.so

razer_chroma_shared_debug: $(RAZER_CHROMA_DEBUG_OBJ)
ifeq ($(LIBDBUS), 0)
	$(CC) -rdynamic -nostdlib -DCREATELIB -shared $(RAZER_CHROMA_DEBUG_OBJ) $(LDFLAGS) -o librazer_chroma_debug.so
endif

razer_chroma_controller_static: $(RAZER_CHROMA_CONTROLLER_OBJ)
ifeq ($(LIBDBUS), 0)
	$(AR) -rs librazer_chroma_controller.a $(RAZER_CHROMA_CONTROLLER_OBJ)
endif

razer_chroma_controller_static_debug: $(RAZER_CHROMA_CONTROLLER_DEBUG_OBJ)
ifeq ($(LIBDBUS), 0)
	$(AR) -rs librazer_chroma_controller.da $(RAZER_CHROMA_CONTROLLER_DEBUG_OBJ)
endif

razer_chroma_controller_shared: $(RAZER_CHROMA_CONTROLLER_OBJ)
ifeq ($(LIBDBUS), 0)
	$(CC) -rdynamic -nostdlib -DCREATELIB -shared $(RAZER_CHROMA_CONTROLLER_OBJ) $(LDFLAGS) -o librazer_chroma_controller.so
	@strip librazer_chroma_controller.so
endif

razer_chroma_controller_shared_debug: $(RAZER_CHROMA_CONTROLLER_DEBUG_OBJ)
	$(CC) -rdynamic -nostdlib -DCREATELIB -shared $(RAZER_CHROMA_CONTROLLER_DEBUG_OBJ) $(LDFLAGS) -o librazer_chroma_controller_debug.so

%.o: %.c 
	$(CC) $(CFLAGS) -c -o $@ $<

%.do: %.c 
	$(CC) $(DEBUG_CFLAGS) -c -o $@ $<

install:
	@echo "::\033[32m INSTALLING librazer_chroma.so\033[0m"
	@echo "===================================="
	@cp librazer_chroma.so /usr/lib
ifeq ($(LIBDBUS), 0)
	@echo "::\033[32m INSTALLING librazer_chroma_controller.so\033[0m"
	@echo "===================================="
	@cp librazer_chroma_controller.so /usr/lib
endif

uninstall:
	@echo "::\033[32m REMOVING librazer_chroma.so\033[0m"
	@echo "===================================="
	@rm /usr/lib/librazer_chroma.so
ifeq ($(LIBDBUS), 0)
	@echo "::\033[32m REMOVING librazer_chroma_controller.so\033[0m"
	@echo "===================================="
	@rm /usr/lib/librazer_chroma_controller.so
endif

clean: 
	rm -f *.do *.o *.a *.so *.da

indent:	
	indent -bap -bli0 -i4 -l79 -ncs -npcs -npsl -fca -lc79 -fc1 -ts4 *.c *.h

