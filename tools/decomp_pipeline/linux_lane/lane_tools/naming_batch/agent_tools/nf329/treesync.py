#!/usr/bin/env python3
"""treesync.py <agent folder> (NF324, 2026-11-24; repo root): keep work/tree (the full include copy the header route grades against) equal
to the CURRENT rebuild/include except for this agent's edited headers (hdr/engine/*.h).
  - an unedited tree file that differs from the repo, or is missing from the tree, is copied from the repo (prints SYNCED);
  - a tree file the repo no longer has is reported (STRAY), not deleted;
  - for an edited header, orig/hdr_<cls>.h must still equal the repo header; otherwise prints STALE (rebase by rerunning hdr_rtti_decls.py
    for that class) and exits 1.
Prints a count line; exit 0 when every edited header's original is current."""
import os, sys, filecmp, shutil
A = sys.argv[1]; tree = f"{A}/work/tree"; repo = "rebuild/include"
edited = {f"engine/{f}" for f in os.listdir(f"{A}/hdr/engine")} if os.path.isdir(f"{A}/hdr/engine") else set()
synced = stray = stale = 0
for root, dirs, files in os.walk(repo):
    for f in files:
        rel = os.path.relpath(os.path.join(root, f), repo); t = os.path.join(tree, rel)
        if rel in edited:
            o = f"{A}/orig/hdr_{os.path.basename(rel)}"
            if not os.path.exists(o) or not filecmp.cmp(o, os.path.join(repo, rel), shallow=False): print("STALE", rel); stale += 1
            continue
        if not os.path.exists(t) or not filecmp.cmp(t, os.path.join(repo, rel), shallow=False):
            os.makedirs(os.path.dirname(t), exist_ok=True); shutil.copy2(os.path.join(repo, rel), t); print("SYNCED", rel); synced += 1
for root, dirs, files in os.walk(tree):
    for f in files:
        rel = os.path.relpath(os.path.join(root, f), tree)
        if not os.path.exists(os.path.join(repo, rel)) and not os.path.islink(os.path.join(root, f)): print("STRAY", rel); stray += 1
print(f"treesync: {synced} synced, {stray} stray, {stale} stale edited headers, {len(edited)} edited")
sys.exit(1 if stale else 0)
