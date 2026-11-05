# Plan phase 7 agent notes (operational)
- No call failed twice with the same arguments.
- Processes: the only process I killed was my own vlandwave run (PID 18975), which shared the work dir vw_a_0 with a concurrent run
  of mine because both used `--tag a`. Both runs were rerun with unique tags (w1, w2). vlandwave.py derives its work dir from
  --tag, so every concurrent run needs its own tag.
- Index: used $S/plan/idx only after the coordinator's refresh at 248ea936 had finished (idx7.log Counter line, no symindex2.py).
  idxpost was rerun afterwards (handback/idxpost).
