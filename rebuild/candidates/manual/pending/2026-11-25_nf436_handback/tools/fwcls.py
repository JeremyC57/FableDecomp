#!/usr/bin/env python3
"""fwcls.py <Class>...: FableWin.pdb LF_CLASS NScript::<Class> (the defining record): base classes (LF_BCLASS resolved to names), methods
with attrs, members with offsets. Index cached in work/fwtypes.pickle (type index -> (line, header line))."""
import sys, re, os, pickle
T = "/tmp/claude-0/egod_pdb/FableWin.types.txt"
W = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
PK = W + "/work/fwtypes.pickle"
if os.path.exists(PK): idx, cls = pickle.load(open(PK, "rb"))
else:
    lines = open(T, encoding="utf-8", errors="replace").read().split("\n")
    idx = {}; cls = {}
    rx = re.compile(r"^\s+(0x[0-9A-F]+) \| (LF_\w+) \[size = \d+\](?: `(.*)`)?")
    cur = None
    for i, l in enumerate(lines):
        m = rx.match(l)
        if m:
            cur = m.group(1); idx[cur] = [m.group(2), m.group(3), []]
            continue
        if cur: idx[cur][2].append(l)
    for k, (kind, nm, body) in idx.items():
        if kind in ("LF_CLASS", "LF_STRUCTURE") and nm and "forward ref" not in " ".join(body[:3]):
            cls[nm] = k
    pickle.dump((idx, cls), open(PK, "wb"))
def name(ti):
    r = idx.get(ti)
    return r[1] if r else ti
def info(C):
    k = cls.get("NScript::" + C) or cls.get(C)
    if not k: return None
    kind, nm, body = idx[k]
    fl = re.search(r"field list: (0x[0-9A-F]+)", " ".join(body)).group(1)
    size = re.search(r"sizeof (\d+)", " ".join(body)).group(1)
    out = {"name": nm, "size": int(size), "bases": [], "methods": [], "members": []}
    fb = idx[fl][2]
    j = 0
    while j < len(fb):
        l = fb[j].strip()
        if l.startswith("- LF_BCLASS"):
            m = re.search(r"type = (0x[0-9A-F]+), offset = (\d+), attrs = (\w+)", fb[j + 1]); out["bases"].append((name(m.group(1)), m.group(3)))
        elif l.startswith("- LF_ONEMETHOD"):
            n = re.search(r"name = `(.*)`", l).group(1); a = re.search(r"attrs = (.*)", fb[j + 1]).group(1)
            out["methods"].append((n, a))
        elif l.startswith("- LF_METHOD"):
            n = re.search(r"name = `(.*)`", l).group(1); ov = re.search(r"overload list = (0x[0-9A-F]+)", l).group(1)
            for ml in idx[ov][2]:
                m = re.search(r"attrs = (.*)", ml)
                if m: out["methods"].append((n, m.group(1)))
        elif l.startswith("- LF_MEMBER"):
            m = re.search(r"name = `(.*)`, Type = (0x[0-9A-F]+)(?: \((.*?)\))?, offset = (\d+), attrs = (\w+)", l)
            out["members"].append((m.group(1), m.group(3) or name(m.group(2)), int(m.group(4)), m.group(5)))
        j += 1
    return out
if __name__ == "__main__":
    for C in sys.argv[1:]:
        r = info(C)
        if not r: print("##", C, "NONE"); continue
        print("##", C, "size", r["size"], "bases", r["bases"])
        for m in r["methods"]: print("  M", m[0], "|", m[1])
        for m in r["members"]: print("  D", m[0], m[1], m[2], m[3])
