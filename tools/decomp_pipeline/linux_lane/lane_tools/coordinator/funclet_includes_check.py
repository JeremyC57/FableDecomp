"""funclet_includes_check.py: report landed sources whose `#include "../src/compiled/..."` points at a missing file.

EH unwind funclet rows (rebuild/src/compiled/01/21/global_Unwind*.cpp) carry no code of their own: they include their parent
row's landed source by path. Renaming a parent's file (a naming landing) silently breaks them. Run this after any landing that
moves files; it prints one line per broken include and always exits 0 (the result is the printed text).

usage: python3 funclet_includes_check.py [repo_root]
"""
import os
import re
import sys

root = sys.argv[1] if len(sys.argv) > 1 else "."
compiled = os.path.join(root, "rebuild", "src", "compiled")
pat = re.compile(r'^\s*#include\s+"\.\./src/compiled/([^"]+)"', re.M)
checked = broken = 0
for dirpath, _, files in os.walk(compiled):
    for name in files:
        if not name.endswith(".cpp"):
            continue
        path = os.path.join(dirpath, name)
        with open(path, encoding="utf-8", errors="replace") as f:
            text = f.read()
        for target in pat.findall(text):
            checked += 1
            if not os.path.exists(os.path.join(compiled, target)):
                broken += 1
                print(f"BROKEN {os.path.relpath(path, root)} -> {target}")
print(f"funclet includes: {checked} checked, {broken} broken")
