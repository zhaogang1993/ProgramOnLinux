#build ipcd

OBJApp:=$(TARGET_PATH)ipcd

GCC:= gcc
GCC_PARA:= -g

#注意引用lib的命名，自定义lib为libtest.so，引用时去掉前缀lib
LIBS:=-pthread -ltest

Srcs = $(wildcard ./*.c)
#指定目标文件
OBJSrc = $(Srcs:.c = .o)

all:$(OBJSrc)
	$(GCC) $(GCC_PARA) -o $(OBJApp) $^ $(LIBS)
clean:
	@echo "This is clean target"
