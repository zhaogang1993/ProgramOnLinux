#!/bin/bash

if [ $# != 1 ]
then
    echo "usage : *.sh PID"
    exit 1
fi

echo "Input pid is $1"

sleep 1
kill -10 $1
kill -25 $1
kill -8 $1
kill -6 $1
kill -35 $1
kill -46 $1
kill -34 $1
