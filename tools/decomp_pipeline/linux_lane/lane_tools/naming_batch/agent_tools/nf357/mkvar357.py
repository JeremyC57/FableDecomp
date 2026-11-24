# mkvar357.py <census_neq.tsv> <tag>: per NEQ emitter row (not CInventoryItemDef), write work/var/<a>_<tag>.cpp = landed HEAD source plus ONE
# line `#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_<T>_SIZE_BODY` (after the GETSUBDEF_BODY define in a BODY TU; directly above the
# `#include "engine/CDefClassBase_GetSubDef.h"` line in a GSB TU). Prints addr kind status.
import sys, re
sys.path.insert(0, '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF357/tools'); import nf
OW = {'CAppearanceDef': '0x0042B0A2', 'CWeaponDef': '0x0042B1A6', 'CEntitySoundDef': '0x0065F604'}
tag = sys.argv[2]
for l in open(sys.argv[1]):
    a, T, sz, mech = l.rstrip('\n').split('\t')[:4]
    if T not in OW: continue
    t = nf.src_of(a).read_text(encoding='utf-8'); M = 'FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_%s_SIZE_BODY' % T.upper()
    line = '#define %s   // NF357: GetSubDef<%s> pick-any copy = size-mode owner %s (scaffolding, not retail)' % (M, T, OW[T])
    if 'BODY' in mech.split():
        k = '#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY'
        m = re.search(r'^%s[^\n]*\n' % re.escape(k), t, re.M); assert m, a
        t = t[:m.end()] + line + '\n' + t[m.end():]
    else:
        k = '#include "engine/CDefClassBase_GetSubDef.h"'
        i = t.index(k); assert t.rfind('\n', 0, i) == i - 1 or i == 0, a
        t = t[:i] + line + '\n' + t[i:]
    open(nf.W / 'var' / ('%s_%s.cpp' % (a, tag)), 'w', encoding='utf-8').write(t); print(a, T, mech, 'WROTE')
