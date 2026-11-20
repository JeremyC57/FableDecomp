#!/bin/bash
# sym.sh <src.cpp> [tag] : grade-free measurement: symbol numbers of the TransformPoint products (loop / sprite)
tag=${2:-$(basename $1 .cpp)}
( cd $W/tools/sched && python3 host.py $W/tools/il/dump2.py $1 $W/tmp/c2p/s_$tag.json >/dev/null 2>&1 )
python3 $W/tools/il/fm.py $W/tmp/c2p/s_$tag.json
