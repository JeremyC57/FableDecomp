# mkvar361.py <kind> <addr...>: variant row texts from the landed HEAD source, written to var/<kind>/<addr>.cpp.
#   R1  + FABLE_RETAIL_CCOUNTEDPOINTER_CCOMPONENT_RESET_SIZE_BODY only (NF352 Q2 isolation run)
#   R   + _RELEASEDATA_SIZE_BODY and _RESET_SIZE_BODY (the NF352 pair), above the row's first #include
#   G   GetDef swap: the row-local primary template CDefinitionManager::GetDef(long, CDefPointer<const T>*) (and the comment block right
#       above it that describes it) is replaced by one comment line; + GETDEF_GLOBAL_BODY, SETDYNAMICCAST_REREAD and the NF349
#       size-body define of each type in GTYPES (env, default CUIMISCTHINGSDEF) above the first #include
#   RG  R + G;  C  the control: G's three defines with the local template LEFT IN (expected: a redefinition error)
import sys, os, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
A = str(nf.A); kind = sys.argv[1]
OWN = {'CUIMISCTHINGSDEF': ('NUISystem::CUIMiscThingsDef', '0x0042B312'), 'CTHINGOBJECTDEF': ('CThingObjectDef', '0x0042B040')}
def rdefs(both):
    L = ['// Pick-any (NF361): engine/CCountedPointer.h size-body opt-ins (NF352; Fidelity: scaffolding, not retail): the out-of-line',
         '//   CCountedPointer<NUISystem::CComponent>::Reset copy takes owner 0x00429C6F\'s optimize("s", on) body' + (', and the ReleaseData copy it calls owner 0x004291DE\'s.' if both else '.')]
    if both: L.append('#define FABLE_RETAIL_CCOUNTEDPOINTER_CCOMPONENT_RELEASEDATA_SIZE_BODY')
    L.append('#define FABLE_RETAIL_CCOUNTEDPOINTER_CCOMPONENT_RESET_SIZE_BODY')
    return L
def gdefs(types):
    L = ['// Pick-any (NF361): CDefinitionManager::GetDef<T>(long, CDefPointer<const T>*) takes the engine/CDefinitionManager.h body',
         '//   (GETDEF_GLOBAL_BODY with the retail re-reading SetDynamicCast) in place of the former same-TU template, and the NF349 size-body',
         '//   opt-in gives the out-of-line ' + ' / '.join('GetDef<%s> copy owner %s\'s' % OWN[t] for t in types) + ' optimize("y", off) / ("s", on) body',
         '//   (Fidelity: scaffolding, not retail).',
         '#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY', '#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD']
    return L + ['#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_%s_SIZE_BODY' % t for t in types]
def drop_template(L):
    for i, l in enumerate(L):
        c = l.split('//')[0]
        if re.search(r'CDefinitionManager::GetDef\s*\(\s*long', c) and not c.rstrip().endswith(';'):
            s = i - 1 if re.match(r'\s*template\s*<\s*class\s+T\s*>\s*$', L[i - 1]) else i
            assert re.search(r'template\s*<\s*class', L[s]), (L[s])
            e = i
            while L[e].rstrip() != '}': e += 1
            c0 = s
            while c0 > 0 and L[c0 - 1].startswith('//') and not L[c0 - 1].startswith('// Leaf:'): c0 -= 1
            com = '\n'.join(L[c0:s])
            if not re.search(r'GetDef', com): c0 = s
            return L[:c0] + ['// (NF361) CDefinitionManager::GetDef<T>(long, CDefPointer<const T>*): engine/CDefinitionManager.h GETDEF_GLOBAL_BODY (see the defines above).'] + L[e + 1:], (c0 + 1, e + 1)
    raise SystemExit('no local template')
types = os.environ.get('GTYPES', 'CUIMISCTHINGSDEF').split(',')
os.makedirs('%s/var/%s' % (A, kind), exist_ok=True)
for a in sys.argv[2:]:
    L = nf.src_of(a).read_text(encoding='utf-8').split('\n'); note = ''
    ins = []
    if kind in ('R1',): ins += rdefs(False)
    if kind in ('R', 'RG'): ins += rdefs(True)
    if kind in ('G', 'RG', 'C'): ins += gdefs(types)
    if kind in ('G', 'RG'): L, rng = drop_template(L); note = 'dropped L%d-L%d' % rng
    k = next(i for i, l in enumerate(L) if l.startswith('#include'))
    L = L[:k] + ins + L[k:]
    open('%s/var/%s/%s.cpp' % (A, kind, a), 'w', encoding='utf-8').write('\n'.join(L))
    print(kind, a, 'inserted %d lines above L%d' % (len(ins), k + 1), note)
