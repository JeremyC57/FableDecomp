#!/bin/bash
# vs.sh <addr> <file>... : grade each file once; print status + count of differing lines
a=$1; shift
for f in "$@"; do
  out=$(python3 $W/tools/g.py $a $f "sweep $(basename $f)" 2>&1)
  echo "$(basename $f): $(echo "$out" | head -1 | awk '{print $NF" "$(NF-1)" "$(NF-2)}' ) diff=$(echo "$out" | grep -c '^[-+] ')"
done
