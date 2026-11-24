#!/usr/bin/env python3
"""survey.py: per ENTRIES Copy row, its jmp target, the catalog rows at both addresses (source, leaf, test), the target row's scaffolding
form, locks, shared header presence. Reads the repo and the retail exe; writes stdout only."""
import os, re, sys, subprocess
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import rdis
R = "/home/user/FableDecomp"; S = os.environ["S"]; A = os.path.join(S, "w22/agentNF337")
cat = open(R + "/rebuild/build_candidates.ps1", errors="replace").read()
blocks = {}
for m in re.finditer(r"\[pscustomobject\]@\{(.*?)\n\s*\}", cat, re.S):
    b = m.group(1); a = re.search(r"Address = '([0-9a-fA-F]+)'", b)
    if not a: continue
    d = dict(re.findall(r"(\w+) = '([^']*)'", b)); blocks[a.group(1).lower()] = d
d, secs = rdis.load()
def lock(a):
    p = os.path.join(S, "w22/nflocks", a, "owner")
    return open(p).read().strip() if os.path.exists(p) else "-"
def leaf(src):
    try:
        for l in open(R + "/rebuild/src/compiled/" + src, errors="replace"):
            if l.startswith("// Leaf:"): return l.split(":", 1)[1].strip()
    except Exception as e: return "ERR"
    return "-"
for l in open(os.path.join(A, "ENTRIES.tsv")):
    a, st, pdb = l.split("\t")[:3]
    cls = re.match(r"\?Copy@(\w+)@@", pdb).group(1)
    b = rdis.read(d, secs, int(a, 16), 16)
    if b[0] == 0xE9:
        tgt = "%08x" % ((int(a, 16) + 5 + int.from_bytes(b[1:5], "little", signed=True)) & 0xffffffff); shape = "jmp"
    else:
        tgt = "-"; shape = "body"
    sb = blocks.get(a, {}); tb = blocks.get(tgt, {})
    hdr = "hdr" if os.path.exists(R + "/rebuild/include/engine/%s.h" % cls) else "nohdr"
    print("\t".join([a, cls, shape, tgt, lock(a), lock(tgt) if tgt != "-" else "-", hdr, sb.get("Source", "?"), leaf(sb.get("Source", "")),
                     tb.get("Source", "-"), leaf(tb["Source"]) if tb else "-"]))
