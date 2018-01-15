#!/bin/bash
find_ret=$(find /home/zhaogang/share/code_new | grep makefile)
NULL=/dev/null
TARGET_PATH=/home/zhaogang/share/code_new/PUBILIC/target/
LIBPATH=/usr/lib/

#将变量什么为全局变量
export TARGET_PATH LIBPATH

for makefile in $find_ret
do
	pushd $(dirname $makefile) > $NULL #重定向到null是否为不输出pushd命令的打印信息
	echo -e "***********************build-begin******************************"
	make -f $makefile
	echo -e "***********************build-end********************************\n"
	popd > $NULL
done

