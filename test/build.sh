#!/bin/bash

rm -f ./hello_world_server
rm -f ./hello_world_client

gcc hello_world_server.c -o hello_world_server -levent
gcc hello_world_client.c -o hello_world_client -levent

echo "build success"
