#!/usr/bin/env python3
"""payload_check.py: for every edited row in the worktree, find the last regrade batch (work/rc<n>/payload.json, highest n
wins) that carried it and compare that batch's source_cpp with the current file. Prints STALE / MISSING rows and a total."""
import os, re, json, glob, subprocess
S = os.environ["S"]; R = os.environ["FABLE_ROOT"]
files = subprocess.run(["git", "-C", R, "diff", "--name-only"], capture_output=True, text=True).stdout.split()
rows = {re.search(r"_([0-9a-f]{8})\.cpp$", f).group(1): f for f in files if f.startswith("rebuild/src/compiled/")}
last = {}
for p in sorted(glob.glob(S + "/mig6/work/rc*/payload.json"), key=lambda p: int(re.search(r"rc(\d+)", p).group(1))):
    for e in json.load(open(p))["result"]["authored"]:
        last[e["address"].lower()] = e["source_cpp"]
bad = 0
for a, f in sorted(rows.items()):
    cur = open(os.path.join(R, f), encoding="utf-8").read()
    if a not in last: print("MISSING", a, f); bad += 1
    elif last[a].replace("\r\n", "\n").rstrip() != cur.replace("\r\n", "\n").rstrip(): print("STALE", a, f); bad += 1
print(len(rows), "edited rows,", bad, "missing or stale")
