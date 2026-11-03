# usage: includers.py <header relative to rebuild/include> [...]  -> landed sources (rebuild/src/compiled rel paths) that
# include any of the headers directly or transitively (resolving "x.h" against the includer's dir, then rebuild/include)
import os, re, sys
INC = "rebuild/include"; SRC = "rebuild/src/compiled"
rx = re.compile(r'^\s*#\s*include\s*[<"]([^>"]+)[>"]', re.M)
cache = {}
def deps(path):
    if path in cache: return cache[path]
    try: txt = open(path, encoding="utf-8", errors="replace").read()
    except OSError: cache[path] = []; return []
    out = []
    for m in rx.findall(txt):
        for cand in (os.path.join(os.path.dirname(path), m), os.path.join(INC, m)):
            if os.path.isfile(cand): out.append(os.path.normpath(cand)); break
    cache[path] = out; return out
targets = {os.path.normpath(os.path.join(INC, h)) for h in sys.argv[1:]}
memo = {}
def reaches(path, stack=()):
    if path in memo: return memo[path]
    if path in targets: memo[path] = True; return True
    if path in stack: return False
    r = any(reaches(d, stack + (path,)) for d in deps(path))
    memo[path] = r; return r
ps = open("rebuild/build_candidates.ps1", encoding="utf-8-sig").read()
for blk in re.findall(r"\[pscustomobject\]@\{(.*?)\n    \}", ps, re.S):
    kv = dict(re.findall(r"(\w+) = '([^']*)'", blk))
    if "Address" in kv and kv.get("Source"):
        p = os.path.normpath(os.path.join(SRC, kv["Source"]))
        if reaches(p): print(kv["Address"].lower() + "\t" + kv["Source"])
