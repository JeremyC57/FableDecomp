# agentP6G tools (all under w22/agentP6G; they call agentP6F's g.sh / hk.sh)
- t.sh <src> "<desc>": grade + append to ATTEMPTS.log.  batch.py <spec.py>: build v/<name>.cpp from a base by exact replacements, grade, print the reload-pair / prologue signature (pairs.py).  Specs: s1.py (whole-function), s2.py (REGION-prologue, both bases), s3.py (REGION-reload1/2), s4.py (score spellings).
- dag.sh <src>: prologue scheduler DAG with priorities.  ch.sh <src>: candidate record traffic (free / new) up to the second split.  sy.sh <src>: c2 symbol-block walk = root candidate order.
- hooks/tsym.py (symbol walk), tben.py (drop test 0x10782AA7), tdrop.py (drops with call chain), tbt.py (candidate creations with call chain).
