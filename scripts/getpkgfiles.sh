#!/bin/bash

SEARCHDIR=$1
SUBSTDIR=$2

find $SEARCHDIR ! -regex ".*[/]\.svn[/]?.*" ! -regex "*~" -type f -print \
 | awk 'index($0,prev"/")!=1 && NR!=1 {print prev} 1 {sub(/\/$/,""); prev=$0}' | sed s_"$SEARCHDIR"_"$SUBSTDIR"_
