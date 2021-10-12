#!/bin/bash

rm -f ./hello_world_server
rm -f ./hello_world_client
rm -f ./compute_file_md5

gcc hello_world_server.c -o hello_world_server -levent
gcc hello_world_client.c -o hello_world_client -levent
g++ compute_file_md5.c -o compute_file_md5 -lcrypto -lssl

echo "build success"
