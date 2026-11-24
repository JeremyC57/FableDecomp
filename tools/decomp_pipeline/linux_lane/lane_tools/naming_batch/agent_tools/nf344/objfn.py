#!/usr/bin/env python3
"""objfn.py <head.obj> <new.obj> (agent NF344): per-function comparison of two objects of one row (same compile path).
Prints the function symbols only one side defines and those whose bytes or relocation lists (offset, symbol, type) differ, then a
summary line `FNDIFF same=N differ=N only_head=N only_new=N` and the non-code section summary (bytes + relocations per section name)."""
import os, sys, collections
ROOT = os.environ.get('FABLE_ROOT', '/home/user/FableDecomp')
sys.path.insert(0, os.path.join(ROOT, 'tools/decomp_pipeline/linux_lane/lane_tools/pk_all')); sys.path.insert(0, os.path.join(ROOT, 'tools/decomp_pipeline'))
sys.dont_write_bytecode = True
import contextlib, io
with contextlib.redirect_stdout(io.StringIO()):
    import pk_all

def load(p):
    syms, secs, raw, relocs = pk_all.coff(p)
    fs = pk_all.functions(syms, secs); out = {}
    for n, (sec, s, e, cls) in fs.items():
        b = raw(sec)[s:e]; rl = [(o - s, t, ty) for (o, t, ty) in relocs(sec) if s <= o < e]
        out[n] = (b, tuple(rl), pk_all.kind_of(secs, sec, cls))
    other = collections.defaultdict(list)
    for k in range(1, len(secs)):
        if secs[k]['flags'] & pk_all.IMAGE_SCN_CNT_CODE: continue
        if secs[k]['name'].startswith('.debug'): continue
        other[secs[k]['name']].append((raw(k), tuple(relocs(k))))
    return out, other

def main():
    h, ho = load(sys.argv[1]); n, no = load(sys.argv[2])
    same = differ = 0
    for k in sorted(set(h) | set(n)):
        if k not in n: print('ONLY_HEAD', h[k][2], len(h[k][0]), k); continue
        if k not in h: print('ONLY_NEW ', n[k][2], len(n[k][0]), k); continue
        if h[k][:2] == n[k][:2]: same += 1
        else:
            differ += 1
            print('DIFFER   ', h[k][2], '%d->%d' % (len(h[k][0]), len(n[k][0])), 'bytes' if h[k][0] != n[k][0] else 'relocs-only', k)
    print('FNDIFF same=%d differ=%d only_head=%d only_new=%d' % (same, differ, len(set(h) - set(n)), len(set(n) - set(h))))
    nd = [s for s in sorted(set(ho) | set(no)) if sorted(ho.get(s, [])) != sorted(no.get(s, []))]
    print('NONCODE_SECTIONS_DIFFER', nd if nd else 'none')

if __name__ == '__main__':
    main()
