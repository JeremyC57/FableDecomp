# fpcheck365.py <H.obj> <G.obj> <ref.obj>...: Step 1 footprint check (NF365). For every function COMDAT the swap CHANGED or ADDED
# (G v H by defining symbol: bytes, relocation list or selection differ, or absent in H), compare G's COMDAT with the same-named COMDAT
# in each reference object (landed GLOBAL_BODY + REREAD + SIZE_BODY rows): bytes, relocation list (offset, symbol, type) and COMDAT
# selection. Prints one line per (symbol, ref): EQUAL / DIFF / NODEF. Also prints ALLSHARED: every COMDAT G shares with a ref by name.
import sys, os, struct
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
def sel(path):
    d = open(path, 'rb').read(); psym, nsym = struct.unpack_from('<II', d, 8); i = 0; out = {}
    while i < nsym:
        o = psym + 18 * i; v, sec, t, scl, na = struct.unpack_from('<IhHBB', d, o + 8)
        if scl == 3 and na and v == 0 and sec > 0:
            a = psym + 18 * (i + 1); out.setdefault(sec, d[a + 14])
        i += 1 + na
    return out
def comdats(path):
    secs, sl = nf.coff(path); s = sel(path); out = {}
    for i, n, v, sec, t, scl in sl:
        if scl == 2 and sec > 0 and (secs[sec - 1]['ch'] & 0x1000) and secs[sec - 1]['name'].startswith('.text') and n not in out:
            x = secs[sec - 1]; out[n] = (x['data'], x['rel'], s.get(sec))
    return out
H, G = comdats(sys.argv[1]), comdats(sys.argv[2]); refs = [(p, comdats(p)) for p in sys.argv[3:]]
chg = [n for n in G if n not in H or H[n] != G[n]]
print('# H=%s G=%s changed/added=%d' % (os.path.basename(sys.argv[1]), os.path.basename(sys.argv[2]), len(chg)))
for n in sorted(chg):
    for p, R in refs:
        if n not in R: print('NODEF\t%s\t%s\tlen=%d' % (os.path.basename(p), n, len(G[n][0]))); continue
        g, r = G[n], R[n]
        v = 'EQUAL' if g == r else 'DIFF bytes=%s relocs=%s sel=%s/%s' % (g[0] == r[0], g[1] == r[1], g[2], r[2])
        print('%s\t%s\t%s\tlen=%d/%d sel=%s' % (v.split()[0] if v == 'EQUAL' else v, os.path.basename(p), n, len(g[0]), len(r[0]), g[2]))
for p, R in refs:
    sh = [n for n in G if n in R]; eq = [n for n in sh if G[n] == R[n]]
    print('ALLSHARED\t%s\tshared=%d equal=%d differ=%s' % (os.path.basename(p), len(sh), len(eq), ','.join(n for n in sh if G[n] != R[n])[:600] or '-'))
