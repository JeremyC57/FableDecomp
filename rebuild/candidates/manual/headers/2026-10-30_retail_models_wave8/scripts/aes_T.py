#!/usr/bin/env python3
"""aes_T.py <addr>...: T of each CActiveEntityScript<T>::SuspendableProcess body from retail: the RTTI-less vtables whose slot 4
is the body (slots 1-3 the microthread's 0x00A44880 / 0x00A44840 / 0x00A447D0, slots 5-6 CActiveEntityScriptBase's 0x00CE1090 /
0x00CE10A0), then every .text dword equal to that vtable (the inline CActiveEntityScript<T> ctor store), then the catalog
name of the function containing it (CEntityScriptBinding<T>::CreateActiveEntityScript). Prints addr, vtable, storers, T."""
import sys, os, re, struct, bisect
sys.argv, args = [sys.argv[0]], sys.argv[1:]
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import vt
R = os.environ["FABLE_ROOT"]
exe = vt.exe
text = [s for s in vt.secs if s[0] == ".text"][0]; rdata = [s for s in vt.secs if s[0] == ".rdata"]
def u32(va): b = vt.rd(va, 4); return struct.unpack("<I", b)[0] if b else None
cat = []
for l in open(R + "/rebuild/oracles/auto-re-candidates.tsv").read().splitlines()[1:]:
    q = l.split("\t"); cat.append((int(q[0], 16), int(q[2]) if q[2].isdigit() else 0, q[1]))
cat.sort(); starts = [c[0] for c in cat]
def fn_of(va):
    i = bisect.bisect_right(starts, va) - 1
    if i >= 0 and cat[i][0] <= va < cat[i][0] + max(cat[i][1], 1): return cat[i]
    return None
def find_dwords(val, sec):
    name, sva, sz, ra, rs = sec; blob = exe[ra:ra + rs]; pat = struct.pack("<I", val); out = []; i = blob.find(pat)
    while i >= 0: out.append(sva + i); i = blob.find(pat, i + 1)
    return out
for a in args:
    A = int(a, 16); res = []
    for sec in rdata:
        for p in find_dwords(A, sec):
            v = p - 16
            if u32(v + 4) == 0x00A44880 and u32(v + 8) == 0x00A44840 and u32(v + 12) == 0x00A447D0 and u32(v + 20) == 0x00CE1090 and u32(v + 24) == 0x00CE10A0:
                res.append(v)
    for v in res:
        st = []
        for p in find_dwords(v, text):
            f = fn_of(p); st.append((p, f))
        Ts = set()
        for p, f in st:
            if f:
                m = re.search(r"CEntityScriptBinding<(.+)>::CreateActiveEntityScript", f[2])
                if m: Ts.add(m.group(1))
        print(a, "%08x" % v, ";".join("%08x:%s" % (p, f[2][:70] if f else "?") for p, f in st), "|".join(sorted(Ts)) or "?", sep="\t")
    if not res: print(a, "novtable", "", "?", sep="\t")
