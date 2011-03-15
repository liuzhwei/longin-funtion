#!/bin/sh
echo"Content-Type:text/plain"
echo
echo"***envairoment variables***"
env
echo"***standard inputs****"
read buf
echo $buf
