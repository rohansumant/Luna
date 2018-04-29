#!/usr/local/bin/python

from sys import argv

def main():
    fname = argv[1]
    f = open(fname,'r')
    op = [i for i in f.read() if i not in [' ','\n','\t']]
    print(''.join(op))
    
main()
    

