# pkdiff362.py <pkdir> <addr> <variant...>: compare pk_all TSVs H_<addr> v <variant>_<addr>: every copy whose verdict, size or reach changes,
# plus the three target copies (0x00429C6F Reset, 0x0042B312 / 0x0042B040 GetDef) and ReleaseData 0x004291DE in each. Flags TURNED_NEQ,
# NEW_NEQ (copy absent in H, NEQ in the variant) and REACH_FLIP (NEQ, reach n -> y). No bytes.
import sys
TG = {'00429c6f': 'Reset', '0042b312': 'GetDef<UIMisc>', '0042b040': 'GetDef<ThingObj>', '004291de': 'ReleaseData'}
def rd(f):
    d = {}
    try: L = open(f).read().splitlines()
    except FileNotFoundError: return None
    for l in L:
        if l.startswith('#') or not l.strip(): continue
        p = l.split('\t')
        if len(p) < 10: continue
        d[(p[2], p[9])] = dict(v=p[0], st=p[1], src=p[3], kind=p[4], size=p[5], rl=p[6], lr=p[7], reach=p[8])
    return d
pk, a = sys.argv[1], sys.argv[2]
H = rd('%s/H_%s.pkall.tsv' % (pk, a))
for v in sys.argv[3:]:
    N = rd('%s/%s_%s.pkall.tsv' % (pk, v, a))
    if N is None: print(a, v, 'MISSING'); continue
    flags = []
    tgt = []
    for k in sorted(set(H) | set(N)):
        h, n = H.get(k), N.get(k)
        if k[0] in TG: tgt.append('%s:%s' % (TG[k[0]], ('%s/%s/%s->%s/%s/%s' % (h['v'], h['size'], h['reach'], n['v'], n['size'], n['reach'])) if h and n else ('H-only %s/%s' % (h['v'], h['size']) if h else 'N-only %s/%s' % (n['v'], n['size'])) ))
        if n and n['v'] == 'NEQ' and (not h): flags.append('NEW_NEQ %s %s' % (k[0], k[1][:70]))
        elif n and h and n['v'] == 'NEQ' and h['v'] != 'NEQ': flags.append('TURNED_NEQ %s %s' % (k[0], k[1][:70]))
        if n and h and n['v'] == 'NEQ' and h['reach'] != 'y' and n['reach'] == 'y': flags.append('REACH_FLIP %s %s' % (k[0], k[1][:70]))
        if h and n and k[0] not in TG and (h['v'], h['size'], h['reach']) != (n['v'], n['size'], n['reach']) and k[0] != 'NOADDR':
            flags.append('CHANGED %s %s %s/%s/%s->%s/%s/%s' % (k[0], k[1][:60], h['v'], h['size'], h['reach'], n['v'], n['size'], n['reach']))
        if h and not n and k[0] != 'NOADDR': flags.append('DROPPED %s %s %s/%s' % (k[0], k[1][:60], h['v'], h['size']))
    print('%s %-12s %s | %s' % (a, v, ' '.join(tgt), '; '.join(flags) or 'no other copy change'))
