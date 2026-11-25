#!/usr/bin/env python3
"""hdrnote425.py: rewrite the `// Header:` note of the remaining local-class CLONE_INLINE rows (batches i1..i4) from the per-branch
trial (work/trialg/trial.tsv): say exactly why engine/<C>.h is not used. Comment-only change; prints a count per reason."""
import sys, os, re, collections
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import lib425 as L
W = L.A
T = collections.defaultdict(list)
for l in open(W + '/work/trialg/trial.tsv'):
    f = l.rstrip('\n').split('\t'); T[f[1]].append(f)
cnt = collections.Counter()
for b in ('i1', 'i2', 'i3', 'i4'):
    for a in open('%s/batches/%s/rows.txt' % (W, b)).read().split():
        p = '%s/batches/%s/rows/%s.cpp' % (W, b, a); s = open(p).read(); C = L.WL[a]['klass']
        base = re.search(r'^class %s : public (\w+)' % re.escape(C), s, re.M).group(1)
        tr = T[a]; why = []
        br = lambda n: ('opt-in %s' % n) if n else 'its default branch'
        for f in tr:
            k = f[3]
            if k == 'NOHDR': why.append('no engine header defines %s' % C); cnt['nohdr'] += 1
            elif k == 'NO_POLY_BRANCH': why.append('engine/%s.h has only a flat byte model of %s (no base, no virtuals), which cannot carry the vftable the inlined copy constructor stores' % (C, C)); cnt['flat'] += 1
            elif k == 'BRANCH_NO_CLONE' or (k == 'NOCOMPILE' and 'C2509' in f[5]):
                why.append('engine/%s.h %s declares the polymorphic %s without Clone, so the real member cannot be defined on it without a header edit' % (C, br(f[4]), C)); cnt['noclone'] += 1
            elif k == 'NOCOMPILE': why.append('engine/%s.h %s does not compile in this prelude (%s)' % (C, br(f[4]), f[5])); cnt['nocompile'] += 1
            elif k == 'TRY' and f[5] == 'leaf=NOLEAF': why.append('engine/%s.h %s declares Clone protected (it defines ?Clone@%s@@MBE...), while the RTTI-placed symbol is %s, so the member defined on it is not this Leaf' % (C, br(f[4]), C, {'U': 'public', 'E': 'private', 'M': 'protected'}[re.search(r'@@([UEM])BE', re.search(r'// Leaf:\s*(\S+)', s).group(1)).group(1)])); cnt['sig'] += 1
            elif k == 'TRY' and f[5].startswith('leaf=LEN'): why.append('engine/%s.h %s does not give retail\'s leaf (leaf length %s bytes)' % (C, br(f[4]), f[5][9:].replace(' v ', ' v retail '))); cnt['len'] += 1
            else: why.append('?'); cnt['?'] += 1
        note = '; '.join(why) + ' (header debt); a local class over its RTTI direct base %s (engine/%s.h).' % (base, base)
        lines = s.split('\n'); i = next(k for k, l in enumerate(lines) if l.startswith('// Header:'))
        j = i + 1
        while j < len(lines) and lines[j].startswith('//   '): j += 1
        lines[i:j] = L.wrap('// Header:', note)
        open(p, 'w').write('\n'.join(lines))
print(cnt)
