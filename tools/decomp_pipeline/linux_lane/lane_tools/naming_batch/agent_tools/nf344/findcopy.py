#!/usr/bin/env python3
"""findcopy.py <obj> <symbol-substring> (agent NF344): search Fable.exe's code for a function copy of an object (relocation fields
wildcarded, 4 bytes each), to find the retail address of a NOADDR copy. Prints each match VA and whether it starts on a 16-byte
boundary; prints no bytes."""
import os, re, sys, struct
ROOT = os.environ.get('FABLE_ROOT', '/home/user/FableDecomp')
sys.path.insert(0, os.path.join(ROOT, 'tools/decomp_pipeline/linux_lane/lane_tools/pk_all')); sys.path.insert(0, os.path.join(ROOT, 'tools/decomp_pipeline'))
sys.dont_write_bytecode = True
import contextlib, io
with contextlib.redirect_stdout(io.StringIO()):
    import pk_all

def main():
    syms, secs, raw, relocs = pk_all.coff(sys.argv[1])
    fs = pk_all.functions(syms, secs)
    hits = [n for n in fs if sys.argv[2] in n]
    d, base, xs = pk_all.exe()
    for n in hits:
        sec, s, e, cls = fs[n]; b = raw(sec)[s:e]
        mask = set()
        for (o, t, ty) in relocs(sec):
            if s <= o < e: mask |= set(range(o - s, o - s + 4))
        pat = b''.join(b'.' if i in mask else re.escape(b[i:i + 1]) for i in range(len(b)))
        rx = re.compile(pat, re.S); found = []
        for va, sz, rp, rs in xs:
            blob = d[rp:rp + rs]
            for m in rx.finditer(blob):
                found.append(base + va + m.start())
                if len(found) > 20: break
        print(n[:140], 'len', len(b), 'matches', len(found), ' '.join('%08x%s' % (f, '' if f % 16 == 0 else '(u)') for f in found[:20]))

if __name__ == '__main__':
    main()
