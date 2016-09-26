#!/bin/bash

START_TIME=`/bin/date +%s -d "2013-01-01 00:00:00"`

sudo /usr/bin/rrdtool create            \
    /var/robot7/r7stats.rrd             \
    --start $START_TIME                 \
    --step 60                           \
    DS:current1:GAUGE:120:0:200         \
    DS:current2:GAUGE:120:0:200         \
    DS:current3:GAUGE:120:0:200         \
    RRA:AVERAGE:0.5:30:1440             \
    RRA:MAX:0.5:30:1440

sudo chown robot7:robot7 /var/robot7/r7stats.rrd
sudo chmod ug+rw /var/robot7/r7stats.rrd
