#!/usr/bin/env python3
"""fwchk434.py <wd> <rows.txt>: for every ??_7 COMDAT a row object emits, compare slot k's relocation symbol with FableWin.pdb's
??_7 of the same spelling, slot k (??_E and ??_G count as one: the weak ??_E alias; FableWin __purecall = _purecall). Prints per vftable
OK / MISMATCH / NOPDB (no FableWin ??_7 of that spelling) and a TOTAL line."""
import sys, glob, importlib.util
sys.path.insert(0, "/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF436/tools")
import fwpub
v2n, n2v = fwpub.load()
sp = importlib.util.spec_from_file_location("cl", "/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF436/tools/coffload.py"); C = importlib.util.module_from_spec(sp); sp.loader.exec_module(C)
def fws(vt, n):
    fv = n2v.get(vt)
    if fv is None: return None
    return [v2n.get(fwpub.follow(fwpub.dw(fv + 4 * i)), "?") for i in range(n)]
def norm(s): return "??_G" + s[4:] if s.startswith("??_E") else ("__purecall" if s in ("__purecall", "_purecall") else s)
tot = {"OK": 0, "MISMATCH": 0, "NOPDB": 0}
wd, rl = sys.argv[1:3]
for a in open(rl).read().split():
    objs = glob.glob(f"{wd}/**/{a}/{a}.obj", recursive=True)
    if not objs: print(a, "NOOBJ"); continue
    for vt, rel in C.vftables(objs[0]):
        rel = sorted(rel); n = len(rel)
        f = fws(vt, n)
        if f is None: tot["NOPDB"] += 1; print(a, vt, "NOPDB", n); continue
        bad = [(o // 4, s, f[o // 4]) for o, s in rel if norm(s) != norm(f[o // 4])]
        k = "MISMATCH" if bad else "OK"; tot[k] += 1
        print(a, vt, k, n, bad if bad else "")
print("TOTAL", tot)
