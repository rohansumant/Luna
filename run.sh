#!/bin/sh

python compress.py $1 > inp
./main < inp
