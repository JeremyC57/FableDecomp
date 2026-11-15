#!/bin/bash
# dag.sh <src> : prologue-region DAG (region containing imul) with priorities
S=/tmp/claude-0/-home-user-FableDecomp/5221bbec-fb18-575f-8649-a32d96bb1c2d/scratchpad; D=$S/w22/agentP6G; T=$S/w22/agentP6F/tools
j=$D/work/dag_$(basename $1 .cpp).json
bash $T/hk.sh $T/sched/t13.py $1 $j '{"pat":["imul"],"target":"FindBestStartVertex"}' >/dev/null
python3 $T/showdag.py $j -v
