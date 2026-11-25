#!/usr/bin/env bash
# chain.sh <batch...>: runpair425 then gates (objgate, pk) for each batch, sequentially
source $S/wine/env.sh >/dev/null; export S; unset TMPDIR TMP
W=$S/w22/agentNF425
for b in "$@"; do
  bash $W/tools/runpair425.sh $b
  python3 $W/tools/gate425.py $b $b > $W/work/og_$b.out 2>&1
  python3 $W/tools/pkgate425.py $b $b > $W/work/pk_$b.out 2>&1
  python3 $W/tools/warncmp.py $b > $W/work/wc_$b.out 2>&1
  echo done > $W/work/chain_$b.done
done
