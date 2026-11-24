#!/usr/bin/env python3
"""verify_nf335.py (NF335; repo root, A and S exported): hand-back checks per done row: rows/<a>.cpp == the graded gen source, rows/<a>_test.cpp
== the graded moved test, the landed source == the graded HEAD copy (pre/<a>/head.cpp) and the landed test == pre/<a>/test.cpp (nothing
landed since), the lock is NF335's, and every orig/tests copy in the batch dirs equals the landed test. Prints a count line."""
import os, sys, glob, filecmp
sys.path.insert(0, "tools/decomp_pipeline"); sys.argv = ["x"]
import naming_status as ns
A, S = os.environ["A"], os.environ["S"]; ent = ns.landed_rows(); W = f"{A}/work/g4"; bad = 0
rows = open(f"{A}/rows.txt").read().split()
for a in rows:
    chk = [(f"{A}/rows/{a}.cpp", f"{W}/gen/{a}.cpp"), (f"{A}/rows/{a}_test.cpp", f"{W}/tests/{a}_test.cpp"),
           ("rebuild/src/compiled/" + ent[a]["Source"], f"{W}/pre/{a}/head.cpp"), ("rebuild/tests/" + ent[a]["TestSource"], f"{W}/pre/{a}/test.cpp")]
    for x, y in chk:
        if not filecmp.cmp(x, y, shallow=False): print("DIFF", a, x, y); bad += 1
    if open(f"{S}/w22/nflocks/{a}/owner").read().strip() != "NF335": print("LOCK", a); bad += 1
for o in glob.glob(f"{A}/batches/*/orig/tests/*_test.cpp"):
    a = os.path.basename(o)[:8]
    if not filecmp.cmp(o, "rebuild/tests/" + ent[a]["TestSource"], shallow=False): print("STALE orig test", a); bad += 1
for o in glob.glob(f"{A}/batches/*/orig/rows/*.cpp"):
    a = os.path.basename(o)[:8]
    if not filecmp.cmp(o, "rebuild/src/compiled/" + ent[a]["Source"], shallow=False): print("STALE orig row", a); bad += 1
print(f"verify_nf335: {len(rows)} rows, {bad} problems")
