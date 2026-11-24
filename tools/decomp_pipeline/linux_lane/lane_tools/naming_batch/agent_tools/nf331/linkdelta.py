# linkdelta.py (NF331) <head link2 txt> <new link2 txt>: duplicate lines (LNK2005 / LNK4006 / LNK1169) per symbol HEAD v new as multisets
# (object + symbol, the "already defined in" object dropped, since the first definer differs by design when the self object changes), the
# unresolved-symbol sets, and the KEPT lines.
import sys, re, collections
def load(p):
    t = open(p).read().splitlines(); d = collections.Counter(); u = set(); k = []; ex = None
    for l in t:
        if l.startswith('exit'): ex = l
        if l.startswith('  ') and re.search(r'LNK2005|LNK4006|LNK1169', l):
            m = re.search(r'(\S+\.obj) : \S+ (LNK\d+): .*?\((\?\S+)\) already defined', l)
            d[(m.group(1), m.group(2), m.group(3)) if m else ('?', '?', l.strip()[-120:])] += 1
        if l.startswith('UNRESOLVED'): u.add(l.split()[-1])
        if l.startswith('KEPT'): k.append(l)
    return ex, d, u, k
eh, dh, uh, kh = load(sys.argv[1]); en, dn, un, kn = load(sys.argv[2])
bysym = lambda d: collections.Counter(s for (o, c, s), n in d.items() for _ in range(n))
sh, sn = bysym(dh), bysym(dn)
print('HEAD', eh, 'duplicate lines', sum(dh.values()), '| new', en, 'duplicate lines', sum(dn.values()))
print('removed by symbol:', dict(sh - sn)); print('added by symbol:', dict(sn - sh))
print('object+symbol lines removed:', sum((dh - dn).values()), 'added:', sum((dn - dh).values()))
print('added lines:', list((dn - dh).items())[:10])
print('unresolved HEAD %d new %d identical=%s' % (len(uh), len(un), uh == un))
print('\n'.join(kh)); print('\n'.join(kn))
