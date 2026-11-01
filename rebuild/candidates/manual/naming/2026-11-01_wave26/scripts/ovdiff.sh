#!/bin/bash
# ovdiff.sh <overlay dir>: unified diff of every overlay file against the repo
ov=$1; cd $ov; find . -type f | sort | while read f; do diff -u /home/user/FableDecomp/${f#./} $f; done
