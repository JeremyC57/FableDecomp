#!/bin/bash
# run.sh src.cpp [-D...] : compile and list calls of Init and the ctor
f=$1; shift
${S}/inlinesplit/cc.sh $f "$@" >/dev/null || { echo FAIL; exit; }
python3 ${S}/inlinesplit/calls.py ${f%.cpp}.asm | grep -A1 "Init\|Run\|??0?\$CFunc1" | grep -v "^--"
