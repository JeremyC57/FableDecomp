#!/usr/bin/env python3
"""tree.py <log.json> <func> <leafnum>... : every sort whose operands contain all given leaf numbers:
site, pass, line, parent node addr, incoming operand leaves, output order."""
import sys, os
sys.dont_write_bytecode = True
sys.path.insert(0, os.environ.get("LKDIR", "/home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/c2probe/lk"))
import lk
log, func = sys.argv[1], sys.argv[2]
want = sys.argv[3:]
name, ev = lk.function_events(lk.load(log), func)
for s in lk.sites(ev):
    labs = [" ".join(lk.leaves(k)) for k in s["kids"]]
    txt = " | ".join(labs)
    toks = set(txt.replace("|", " ").split())
    if all(any(t.split(":")[0].lstrip("*").rstrip("=>") in (w,) or t.split(":")[0].rstrip("=>") == w for t in toks) for w in want):
        out = [labs[p] if p is not None else "?" for p in s["perm"]]
        print("%5d %s L%s @%x in[%s] out[%s]" % (s["n"], s["pass"], s["line"], s["parent"], txt, " | ".join(out)))
