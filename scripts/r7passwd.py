#!/usr/bin/python2

import sys
import hashlib

if ( len(sys.argv) == 2 ) :
    raw = sys.argv[1]
    hashed = (hashlib.sha256(raw)).hexdigest()
    print hashed
else :
    print 'usage: r7passwd.py password'
