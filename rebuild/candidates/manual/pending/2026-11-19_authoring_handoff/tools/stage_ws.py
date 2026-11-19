#!/usr/bin/env python3
"""stage_ws.py <workspace> <dest>: copy an author workspace for the hand-off without retail bytes, retail disassembly, binaries or bulk scratch."""
import os, re, shutil, sys, fnmatch
src, dst = sys.argv[1:3]
SKIP_DIRS = {"lv", "wine", "mt", "tc", "__pycache__", "q", "work_obj"}
SKIP_FILES = ["oracle*.tsv", "*_oracle.tsv", "retail_dis*", "*retail*.txt", "*.dis", "*.obj", "*.exe", "*.lib", "*.pdb", "*.ilk", "*.dll",
              "*.bin", "*.o", "*.a", "*.7z", "*.zip", "*.gz", "*.cod"]
MAX = 600 * 1024
disline = re.compile(r"^\s*(?:0x)?[0-9a-fA-F]{6,8}:?\s+(?:[0-9a-fA-F]{2}\s?){1,15}\s+[a-z]", re.M)
hexrow = re.compile(r"(?:^|\t)[0-9a-fA-F]{400,}(?:\t|$)", re.M)
kept = skipped = 0
for root, dirs, files in os.walk(src):
    dirs[:] = [d for d in dirs if d not in SKIP_DIRS]
    for f in files:
        p = os.path.join(root, f); rel = os.path.relpath(p, src)
        if any(fnmatch.fnmatch(f, g) for g in SKIP_FILES) or os.path.getsize(p) > MAX:
            skipped += 1; continue
        try:
            t = open(p, "rb").read()
        except OSError:
            skipped += 1; continue
        s = t.decode("latin1")
        if b"\0" in t[:4096] or hexrow.search(s) or len(disline.findall(s)) > 40:   # binary, hex byte column, or a full disassembly listing   # binary, or a long hex byte column (retail bytes)
            skipped += 1; continue
        q = os.path.join(dst, rel); os.makedirs(os.path.dirname(q), exist_ok=True); shutil.copy2(p, q); kept += 1
print(f"{src} -> {dst}: kept {kept}, skipped {skipped}")
