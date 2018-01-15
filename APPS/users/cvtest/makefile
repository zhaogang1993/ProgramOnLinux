#编译opencv进程的makefile

OBJApp:=$(TARGET_PATH)cvtestd

#源文件--cpp文件
Srcs = $(wildcard ./*.cpp)
#指定目标文件
OBJSrc = $(Srcs:.cpp = .o)
#指定编译宏
#DBUILD = -DSYSCALL_SIGNAL_API
#DBUILD = -DSYSV_SIGNAL_API
#DBUILD = -DBSD_SIGNAL_API
#DBUILD = -DNO_JOIN


#定义动态库
Libs = -lstdc++ -rdynamic -pthread -lsqlite3

#定义opencv的编译参数
Param = `pkg-config opencv --cflags --libs`

# $@表示目标     $^表示所有依赖 
# | 符号前是常规依赖,之后是order-only依赖,order-only依赖表示
# 目标的重建只需要部分依赖就行了(常规依赖)---在此处行不通，
# 每次重建zhaod都需要重新引用public里面的文件
# 此order-only依赖可用于lib库
#$(OBJApp):$(OBJSrc) | $(OBJPub)
all:$(PubObj) $(OBJSrc)
	g++ -Wall -g -o $(OBJApp) $^ $(Param) $(Libs) $(DBUILD)
clean:
	echo "This is clean target."
