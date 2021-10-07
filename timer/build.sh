#!/bin/bash

rm -f ./timer.out

gcc main.c ngx_rbtree.c rbtimer.c -o timer.out

echo "build success"
