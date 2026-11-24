# mkmodel.py <Class...>: CCreatureActionBase-derived class models for the Clone family (NF323, 2026-11-24; NF322 recipe, NF320 Q1).
# Overrides = evidence/ovlist_*.txt slots (retail vftable labels not in the RTTI base chain), declarations copied from the base
# engine/CCreatureActionBase.h declaration of the same name (`=0` dropped), public (every label is U..). No own members (sizeof 0xa8,
# the Clone's `push 0xa8`). Flat GENERATED header -> `#ifdef FABLE_RETAIL_<CLASS>_VIRTUAL_MODEL` new / `#else` the old text unchanged,
# inserted after `#include "rebuild_abi.h"`; no header -> a new engine/<Class>.h (declarations only, like CCreatureAction_AlertToPlayer.h).
# Writes work/tree/engine/<Class>.h (and orig/hdr/engine/<Class>.h when the repo has one).
import sys, re, os, shutil, glob
A = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF328'; R = '/home/user/FableDecomp/rebuild/include/engine/'
base = open(R + 'CCreatureActionBase.h').read()
ov = {}
for f in glob.glob(A + '/evidence/ovlist_*.txt'):
    cur = None
    for l in open(f):
        if not l.startswith(' '): cur = l.split()[0]; ov[cur] = []
        else:
            p = l.split(); ov[cur].append((int(p[1]), p[2], p[3] + (' (FableWin.pdb, Ego_r.pdb)' if p[4] == 'eg=1' else ' (FableWin.pdb)')))
def decl(name):
    m = re.findall(r'^\s*(virtual [^;\n]*\b%s\s*\([^;\n)]*\)(?:\s*const)?)\s*(?:=\s*0)?\s*;' % re.escape(name), base, re.M)
    assert len(m) == 1, (name, m)
    return m[0].strip() + ';'
for C in sys.argv[1:]:
    up = C.upper(); mac = 'FABLE_RETAIL_%s_VIRTUAL_MODEL' % up
    sl = [x for x in ov[C] if x[0] != 0]
    vt = None
    for l in open('/home/user/FableDecomp/rebuild/manifest/rtti-vtables.tsv'):
        p = l.split('\t')
        if p[0] == C: vt = p[1]; break
    cm = ['// NF323 (2026-11-24, NF320 Q1 / NF322 recipe): the polymorphic retail class for the Clone TU, RTTI .?AV%s@@ : CCreatureActionBase' % C,
          '// (rtti_bases.tsv), no own members (retail sizeof 0xA8: the Clone allocates 0xa8 and copies only the base). Declarations only. Each',
          '// virtual overrides the CCreatureActionBase virtual at the same slot of retail vftable 0x%s (RTTI-port label + the PDB name sets):' % vt.upper()]
    for k, t, lab in sl:
        cm.append('//   slot %d = 0x%s %s' % (k, t, lab))
    cls = ['class %s : public CCreatureActionBase {' % C, 'public:'] + ['    ' + decl(re.match(r'\?(\w+)@', lab.split()[0]).group(1)) for k, t, lab in sl] + ['};',
           'FABLE_STATIC_ASSERT(sizeof(%s) == 0xa8);' % C]
    h = R + C + '.h'; out = A + '/work/tree/engine/' + C + '.h'
    if os.path.exists(h):
        os.makedirs(A + '/orig/hdr/engine', exist_ok=True); shutil.copy(h, A + '/orig/hdr/engine/' + C + '.h')
        L = open(h).read().split('\n'); i = L.index('#include "rebuild_abi.h"')
        g = max(j for j, l in enumerate(L) if l.startswith('#endif'))
        new = L[:i + 1] + ['#ifdef ' + mac] + cm + ['#include "engine/CCreatureActionBase.h"'] + cls + ['#else'] + L[i + 1:g] + ['#endif // ' + mac] + L[g:]
    else:
        g = 'FABLE_ENGINE_%s_H' % up
        new = ['#ifndef ' + g, '#define ' + g] + cm + ['// New header: no class model existed in rebuild/include (NF323). Needs FABLE_RETAIL_CCREATUREACTIONBASE (the retail base).',
               '#include "rebuild_abi.h"', '#include "engine/CCreatureActionBase.h"'] + cls + ['#endif // ' + g, '']
    open(out, 'w').write('\n'.join(new)); print(C, 'existing' if os.path.exists(h) else 'NEW', len(sl), 'decls')
