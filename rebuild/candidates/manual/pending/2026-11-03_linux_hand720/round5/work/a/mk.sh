#!/bin/bash
# mk.sh out.cpp body.cpp  (from work/a)
A=$(dirname $0); cat $A/head.h $A/models.h $2 > $1
