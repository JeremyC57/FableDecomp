# source this before running the lk tools: needs S (the lane scratchpad) set; runs go to $C2PRIO_RUNS (default $S/c2lk_runs).
: "${S:?set S to the lane scratchpad first}"
source $S/wine/env.sh >/dev/null 2>&1
export PYTHONDONTWRITEBYTECODE=1
export C2PRIO_RUNS=${C2PRIO_RUNS:-$S/c2lk_runs}
