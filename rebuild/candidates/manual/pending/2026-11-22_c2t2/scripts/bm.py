#!/usr/bin/env python3
"""bm.py <log.json>: bookmark AND (op 0x27d) / OR (0x27e) trees: operands IND(temp => sym + 0x2814) = C (CurrentBookmarkMask)
and IND(temp => sym + 8) = M (state BookmarkMask).  One line per tree (parent node), at its LAST sort: site#, pass, line,
op, output roles, keys, base-symbol numbers, temp numbers, how many sorts the tree had."""
import sys, os
sys.dont_write_bytecode = True
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline/linux_lane/lane_tools/c2probe/lk")
import lk
name, ev = lk.function_events(lk.load(sys.argv[1]), None)
ss = lk.sites(ev)
def role(k):
    # k = IND desc; sub[0] = temp leaf desc whose sub[0] = value (sym + const)
    try:
        t = k[6][0]; v = t[6][0]
        c = [x for x in v[6] if x[2] == 7][0][5][0]
        s = [x for x in v[6] if x[2] != 7][0][5][1]
        r = {0x2814: "C", 8: "M"}.get(c)
        return r, s, t[5][1]
    except Exception:
        return None, None, None
last, cnt = {}, {}
for s in ss:
    if s["op"] in (0x27d, 0x27e) and len(s["kids"]) == 2:
        rs = [role(k) for k in s["kids"]]
        if all(r[0] for r in rs) and {r[0] for r in rs} == {"C", "M"}:
            last[s["parent"]] = (s, rs); cnt[s["parent"]] = cnt.get(s["parent"], 0) + 1
for p, (s, rs) in sorted(last.items(), key=lambda x: x[1][0]["n"]):
    out = [rs[i][0] for i in s["perm"]]
    keys = [s["kids"][i][3] for i in s["perm"]]
    print(f"{s['n']:6d} {s['pass']} L{s['line']:<4} {'AND' if s['op']==0x27d else 'OR '} {''.join(out)}  keys {keys[0]:08x} {keys[1]:08x}  "
          f"base s{rs[0][1] if rs[0][0]=='C' else rs[1][1]}(C) s{rs[0][1] if rs[0][0]=='M' else rs[1][1]}(M)  temps {rs[0][2]},{rs[1][2]}  in {''.join(r[0] for r in rs)}  sorts {cnt[p]}")
