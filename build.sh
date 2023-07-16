#!/bin/bash

file=$1
machine=$2

echo "============================Building $file====================================="
if [ $machine == 'mac' ]
then
    g++ $file -framework OpenCL -o a.out
else
    g++ $file -lOpenCL -o a.out
fi

if [ $? != 0 ]
then
    echo '!!!!!!Failed!!!!!!!!'
    exit;
else
echo "=============================Running $file====================================="
./a.out
fi