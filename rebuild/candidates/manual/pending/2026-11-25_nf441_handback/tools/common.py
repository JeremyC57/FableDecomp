"""common.py: shared loaders for NF441 tools (retail PE, repo tables)."""
import sys, struct, os, re, functools
S = "/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad"
R = "/home/user/FableDecomp"
sys.path.insert(0, S + "/w22/agentNF427/tools")
import pe
EXE = pe.EXES["retail"]
D, BASE, SECS = pe.load(EXE)
TEXT_LO = BASE + SECS[0][0]; TEXT_HI = TEXT_LO + SECS[0][1]
def rd(va, n): return pe.read_va(EXE, va, n)
def dw(va):
    b = rd(va, 4); return struct.unpack("<I", b)[0] if len(b) == 4 else None
def is_text(v): return v is not None and TEXT_LO <= v < TEXT_HI
def tsv(p):
    return [l.split("\t") for l in open(p, encoding="utf-8").read().splitlines()[1:] if l.strip()]
@functools.lru_cache(None)
def tables():
    sym2a, a2sym = {}, {}
    for f in tsv(R + "/rebuild/oracles/reloc_bindings.tsv"):
        if len(f) > 1:
            a = int(f[1], 16); sym2a.setdefault(f[0], set()).add(a); a2sym.setdefault(a, set()).add(f[0])
    selfs = {}
    for f in tsv(R + "/rebuild/oracles/self_symbols.tsv"):
        if len(f) > 1 and f[1]: selfs[int(f[0], 16)] = f[1]
    return sym2a, a2sym, selfs
def name_of(a):
    sym2a, a2sym, selfs = tables()
    s = selfs.get(a)
    b = sorted(a2sym.get(a, ()))
    return s, b
def t3_libpk():
    P = R + "/rebuild/candidates/manual/pending/2026-11-25_nf430_handback/T3.txt"
    out = []
    for l in open(P):
        f = l.rstrip("\n").split("\t")
        if len(f) > 3 and f[2] == "libpk_hold":
            m = re.search(r"vftable\(s\) ([0-9A-Fx, ]+):", f[3])
            vas = [int(x, 16) for x in m.group(1).replace(" ", "").split(",")] if m else []
            out.append((int(f[1], 16), vas))
    return out
