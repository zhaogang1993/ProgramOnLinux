#编译lib的makefile文件

OBJso:=$(LIBPATH)libtest.so
src:=*.c

GCCFLAG:=-Wall -shared -fPIC
LIBS:=-ldl
CURPATH:=$(shell pwd)
src:=$(CURPATH)/*.c
#.o文件
OBJSrc = $(src:.c = .o)

all:$(src)
	gcc $(GCCFLAG) -o $(OBJso) $^ $(LIBS)
clean:
	@echo "This is lib clean makefile"