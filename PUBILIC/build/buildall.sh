#!/bin/bash
find_ret=$(find /home/zhaogang/share/code_new | grep makefile)
NULL=/dev/null
TARGET_PATH=/home/zhaogang/share/code_new/PUBILIC/target/
LIBPATH=/usr/lib/

#������ʲôΪȫ�ֱ���
export TARGET_PATH LIBPATH

for makefile in $find_ret
do
	pushd $(dirname $makefile) > $NULL #�ض���null�Ƿ�Ϊ�����pushd����Ĵ�ӡ��Ϣ
	echo -e "***********************build-begin******************************"
	make -f $makefile
	echo -e "***********************build-end********************************\n"
	popd > $NULL
done

