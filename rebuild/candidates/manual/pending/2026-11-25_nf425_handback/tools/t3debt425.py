#!/usr/bin/env python3
"""t3debt425.py <accepted batches...>: append the debt booked by the ACCEPT batches to T3.txt (after the residue lines; idempotent:
everything after the '# --- debt' marker is rewritten)."""
import sys, os, re, glob, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import lib425 as L
A = L.A; bs = sys.argv[1:]
t3 = open(A + '/T3.txt').read().split('# --- debt')[0].rstrip('\n') + '\n'
out = ['# --- debt booked by the ACCEPT batches (address = the landed row it hangs on)']
def rows(b):
    return [l.rstrip('\n').split('\t') for l in open('%s/batches/%s/RESULTS.tsv' % (A, b))][1:]
acc = {}
for b in bs:
    for r in rows(b):
        if r[1] == 'ACCEPT': acc[r[0]] = (b, r[3])
# 1. SHARED_HEADER_AVAILABLE (local class while engine/<C>.h exists)
for b in bs:
    for l in list(open('%s/batches/%s/CHECKS.tsv' % (A, b)))[1:]:
        f = l.rstrip('\n').split('\t')
        if f[0] in acc and 'SHARED_HEADER_AVAILABLE' in f[-1]:
            C = L.WL[f[0]]['klass']
            ht = open(L.R + '/rebuild/include/engine/%s.h' % C).read()
            cp = bool(re.search(r'\bCopy\s*\(', ht)); op = bool(re.search(r'operator\s*=\s*\(', ht))
            why = ('declares neither Copy nor operator=' if not cp and not op else 'declares Copy but no out-of-line operator= (the compiler would generate its own operator= instead of calling the retail out-of-line one)' if cp and not op
                   else 'declares operator= but not Copy' if op and not cp else 'declares both, but its branch does not compile in this prelude (C2144)')
            out.append('nf425\t0x%s\theader %s\tSHARED_HEADER_AVAILABLE (batch %s): the row declares Copy on a local class %s over its RTTI direct base header because engine/%s.h %s; move the row onto engine/%s.h once a polymorphic branch there declares `virtual void Copy(const CDefClassBase*)` and `%s& operator=(const %s&)` (NF417 / NF423 r2 header debt)' % (f[0].upper(), C, b, C, C, why, C, C, C))
# 2. CLONE_INLINE vftable opt-ins and the NF424 ??_G overlap
g424 = {}
for a in open(A + '/work/nf424_g_definers.txt').read().split():
    p = L.nf.src_of(a); m = re.search(r'// Leaf:\s*\?\?_G(\w+)@@', open(p).read()) if p else None
    if m: g424[m.group(1)] = a
for b in bs:
    if not b.startswith('i'): continue
    for a in sorted(x for x, (bb, _) in acc.items() if bb == b):
        C = L.WL[a]['klass']; extra = ''
        if C in g424: extra = '; NF424 landed ??_G%s at 0x%s on engine/%s.h without Clone: the two ??_7%s@@6B@ pick-any texts differ (link 0 duplicates either way; the first object wins); one class model should serve both rows' % (C, g424[C].upper(), C, C)
        out.append('nf425\t0x%s\tvftable %s\tCLONE_INLINE local class %s (batch %s): ??_7%s@@6B@ is pick-any and names the base virtuals plus Clone (and PerformAction where the local class declares it; evidence/vtcmp.tsv); the remaining overrides (FinishAction, the class own virtuals) need %s (the NF329 VIRTUAL_MODEL form the h1/h2 rows use, declaring Clone with the RTTI access); then this row moves onto the header like h1/h2 (RULINGS_NEEDED r1)%s' % (a.upper(), C, C, b, C, ('a polymorphic branch of engine/%s.h' % C) if os.path.exists(L.R + '/rebuild/include/engine/%s.h' % C) else ('an engine/%s.h (none exists)' % C), extra))
# 2b. HOLD rows (batches/ihold)
for l in list(open(A + '/batches/ihold/RESULTS.tsv'))[1:]:
    f = l.rstrip('\n').split('\t')
    out.append('nf425\t0x%s\tport CLONE_INLINE\tHOLD (batches/ihold): %s; RTTI self %s' % (f[0].upper(), f[4].split(': ', 1)[1], f[3]))
# 3. NAME_FIXES owners (stand-in operator= still landed)
for l in list(open(A + '/NAME_FIXES.tsv'))[1:]:
    f = l.rstrip('\n').split('\t')
    out.append('nf425\t0x%s\trespell %s\tNAME_FIXES: the landed owner row still defines %s; the coordinator respells it (and its Leaf) to %s so the NF425 Copy rows resolve (unresolved before and after in the /FORCE link)' % (f[0].upper(), f[1], f[3], f[1]))
# 4. Option S units
for b in bs:
    p = '%s/batches/%s/UNITS.tsv' % (A, b)
    for l in list(open(p))[1:]:
        f = l.rstrip('\n').split('\t')
        if len(f) > 2 and f[2] == 'S':
            out.append('nf425\t0x%s\tunit %s\tOption S thin row (batch %s): Copy is compiled in the owner TU 0x%s (both PDBs: tc_register.obj); keep the thin row and its owner in one unit' % (f[0].upper(), f[1], b, f[1].upper()))
# 5. T3 queue lines resolved
RES = ['004536A9', '00453810', '00453899', '00453BE3', '00453C1B', '00453CE5', '00454038', '0045417E', '0045473A', '0045C549', '0045D500', '0045FEDE', '004600C6', '0046244C', '00462B10']
for a in RES:
    if a.lower() not in acc: continue
    out.append('nf425\t0x%s\tresolved\tT3_PORT_QUEUE line (nf324%s) resolved: the row lands here as the real RTTI member (batch %s)' % (a, ' and nf386' if a == '0045473A' else '', acc[a.lower()][0]))
open(A + '/T3.txt', 'w').write(t3 + '\n'.join(out) + '\n')
print('debt lines', len(out) - 1, collections.Counter(x.split('\t')[2].split()[0] for x in out[1:]))
