#!/usr/bin/env python3
"""codavt430.py: match retail RTTI-less vftables to GameCODA SDK lib vftables (??_7 symbols + slot relocations),
scoring each slot by masked byte equality of the lib slot function body against retail at the slot address.
usage: codavt430.py <lib>... -- <vt_class.tsv>   -> stdout TSV"""
import sys, struct
sys.path.insert(0, "/home/user/FableDecomp/tools/decomp_pipeline/libsig")
sys.path.insert(0, "/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF427/tools")
import coff_lib, pe
args = sys.argv[1:]
k = args.index("--")
libs, vtf = args[:k], args[k + 1]
R = pe.EXES["retail"]
libvts = {}   # name -> [slot names]
funcs = {}    # name -> (bytes, mask)
for lib in libs:
    ar = coff_lib.Archive(lib)
    for m in ar.members:
        if m.kind != "coff": continue
        try: o = m.coff()
        except Exception: continue
        for cs in o.code_sections():
            nm = cs.primary
            b = bytearray(cs.data); mask = bytearray(b"\1" * len(b))
            for r in cs.relocs:
                for j in range(r.offset, min(r.offset + r.size, len(b))): mask[j] = 0
            if nm not in funcs or len(funcs[nm][0]) < len(b):
                funcs[nm] = (bytes(b), bytes(mask))
        for s in o.symbols:
            if s.name.startswith("??_7") and s.section > 0:
                sec = o.sections[s.section - 1]
                nxt = sorted(x.value for x in o.symbols if x.section == s.section and x.value > s.value and x.storage in (2, 3))
                end = nxt[0] if nxt else sec["size"]
                rel = sorted((r.offset, r.symbol) for r in o.relocs(sec) if s.value <= r.offset < end)
                slots = [n for off, n in rel]
                if s.name not in libvts or len(libvts[s.name]) < len(slots):
                    libvts[s.name] = slots
def dw(va):
    b = pe.read_va(R, va, 4); return struct.unpack("<I", b)[0] if len(b) == 4 else 0
def match(name, va):
    f = funcs.get(name)
    if not f: return None
    b, m = f
    rb = pe.read_va(R, va, len(b))
    if len(rb) < len(b): return False
    return all((not m[i]) or b[i] == rb[i] for i in range(len(b)))
sys.stderr.write("lib vftables %d, funcs %d\n" % (len(libvts), len(funcs)))
for l in open(vtf):
    f = l.rstrip("\n").split("\t")
    va = int(f[0], 16); n = int(f[2])
    rs = [dw(va + 4 * i) for i in range(n)]
    best = []
    for name, slots in libvts.items():
        if not slots: continue
        L = min(len(slots), n)
        if len(slots) > n: continue
        ok = sum(1 for i in range(L) if match(slots[i], rs[i]))
        bad = sum(1 for i in range(L) if match(slots[i], rs[i]) is False)
        if ok: best.append((ok - bad, ok, bad, len(slots), name))
    best.sort(reverse=True)
    top = best[:3]
    print("%s\t%s\t%d\t%s" % (f[0], f[1], n, " | ".join("%s ok=%d bad=%d ls=%d" % (x[4], x[1], x[2], x[3]) for x in top)))
