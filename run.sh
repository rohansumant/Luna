#!/bin/sh

python3 compress.py $1 > inp
./main < inp
