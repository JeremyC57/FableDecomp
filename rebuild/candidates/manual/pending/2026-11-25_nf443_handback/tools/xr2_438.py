#!/usr/bin/env python3
"""xr2_438.py <batch...>: second-opinion callee binding check. For each renames.tsv line, every relocation to the NEW symbol in the new
object's .text sections whose size equals the retail row size is resolved against the retail bytes (REL32 -> call target, DIR32 ->
value); OK when all resolve to the target and the old symbol appears in no relocation. Rewrites BAD lines of evidence/xrcheck.txt."""
import sys, os, glob, struct, csv
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
W = str(nf.A)
for b in sys.argv[1:]:
    B = W + '/batches/' + b; xr = open(B + '/evidence/xrcheck.txt').read().splitlines(); out = []
    for l in xr:
        f = l.split('\t')
        if len(f) < 2 or f[1] != 'BAD': out.append(l); continue
        a = f[0]; ren = [r for r in csv.reader(open(B + '/renames.tsv'), delimiter='\t') if r[0] == a]
        o = glob.glob('%s/work/wd_%sn/*/*/%s/%s.obj' % (W, b, a, a))[0]; secs, sl = nf.coff(o)
        retail = bytes.fromhex(nf.catalog()[a]['bytes']); va0 = int(a, 16)
        for _, old, new, t in ren:
            if new not in l: continue
            tg = set(); oldseen = False
            for s in secs:
                if not s['name'].startswith('.text'): continue
                for off, sym, typ in s['rel']:
                    if sym == old: oldseen = True
                    if sym == new and len(s['data']) == len(retail):
                        x = struct.unpack_from('<I', retail, off)[0]
                        tg.add('%08x' % (((va0 + off + 4 + x) & 0xffffffff) if typ == 0x14 else x))
            ok = tg == {t} and not oldseen
            out.append('%s\t%s\t%s -> %s\t%s\t(xr2_438: relocations of the new symbol in the retail-sized .text section, resolved against retail)' % (a, 'OK' if ok else 'BAD', new, ','.join(sorted(tg)) or 'none', t))
    open(B + '/evidence/xrcheck.txt', 'w').write('\n'.join(out) + '\n')
    print(b, sum(1 for l in out if '\tBAD\t' in l), 'BAD')
