#!/bin/bash

export R7DB='/var/lib/robot7/r7db.sqlite'

sudo echo > $R7DB
sudo sqlite3 $R7DB 'VACUUM;'
sudo chown robot7:http $R7DB
sudo chmod ug+rw $R7DB

sqlite3 $R7DB < r7db-sqlite.schema
sqlite3 $R7DB < r7db-sqlite.data
