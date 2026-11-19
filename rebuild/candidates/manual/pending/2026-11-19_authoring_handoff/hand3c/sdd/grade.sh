#!/bin/bash
# grade.sh <dir> : grade every row source in dir against the catalog oracle
for f in $1/*.cpp; do a=$(echo $f | grep -o '00[0-9a-f]\{6\}\.cpp' | cut -c1-8); leaf=$(grep -m1 '^// Leaf:' $f | awk '{print $3}'); fl=$(grep -m1 '^// Flags:' $f | awk '{print $3}'); [ "$fl" = none ] && fl=; [ -n "$fl" ] && fl="--flags $fl"
 r=$(python3 tools/decomp_pipeline/linux_lane/lane_tools/q.py $a $f $leaf $fl 2>&1 | grep -m1 -o "MATCH\|RELOCATION_MATCH\|DIFFER([0-9v]*)\|SRC_FAIL\|[A-Z_]*FAIL[A-Z_]*"); echo "$a $r"; done
