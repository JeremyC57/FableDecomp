#!/usr/bin/env python3
"""nvsurvey.py <files>: for each row's local CGameScriptInterface block, print the non-virtual residue (after macro
expansion and removing virtual declarations), normalised, for clustering."""
import sys, os, re, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import mig_cgsi as M
from common import find_block, norm
c = collections.Counter(); ex = {}
for f in sys.argv[1:]:
    t = open(f, encoding="utf-8").read()
    b = find_block(t, "CGameScriptInterface")
    n = norm(t[b[0]:b[1]])
    m = re.match(r"^(?:class|struct) CGameScriptInterface ?\{ ?(.*)\};$", n)
    body = M.expand_macros(t, m.group(1)) if m else n
    res = M.DECL.sub("", body).replace("public:", "").strip()
    items = [x.strip() for x in res.split(";") if x.strip()]
    for it in items:
        k = re.sub(r"\b\d+\b|0x[0-9a-fA-F]+", "N", it)
        c[k] += 1; ex.setdefault(k, f)
    print("ROW", f, len(items), "|", " ; ".join(items)[:200])
for k, v in c.most_common(60): print("ITEM", v, k[:150])
