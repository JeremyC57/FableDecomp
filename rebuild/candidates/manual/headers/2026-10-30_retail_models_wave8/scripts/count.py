#!/usr/bin/env python3
"""count.py <class>...: rows under rebuild/src/compiled defining `class|struct <X> {` (template prefix allowed); -l lists files."""
import sys, re, os, glob
R = os.environ["FABLE_ROOT"]; lst = "-l" in sys.argv
files = [f for f in glob.glob(R + "/rebuild/src/compiled/**/*.cpp", recursive=True) if not f.endswith(".pre.cpp")]
texts = {f: open(f, encoding="utf-8", errors="replace").read() for f in files}
for c in [a for a in sys.argv[1:] if a != "-l"]:
    rx = re.compile(r'^[ \t]*(?:template\s*<[^>{;]*>\s*)?(?:class|struct)\s+' + re.escape(c) + r'\b[^;{]*\{', re.M)
    hit = sorted(os.path.relpath(f, R) for f, t in texts.items() if rx.search(t))
    print(c, len(hit))
    if lst: print("\n".join(hit))
