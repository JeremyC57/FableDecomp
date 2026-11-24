# mkvar365.py <addr...>: the NF361 swap text (generator form of agent_tools/nf361/mkvar361.py kind G, unchanged wording) from the landed
# HEAD source, written to var/G/<addr>.cpp. The NF349 size-body defines are chosen from the GetDef<T> DEF symbols of the row's HEAD
# object work/obj/H_<addr>.obj (types with an NF349 block: CThingBaseDef, CThingObjectDef, NUISystem::CUIMiscThingsDef, CExpressionDef);
# other GetDef<T> instances get GETDEF_GLOBAL_BODY + REREAD only. Prints the chosen types and the dropped line range.
import sys, os, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__))); import nf
SB = {'??$GetDef@VCUIMiscThingsDef@NUISystem@@@': 'CUIMISCTHINGSDEF', '??$GetDef@VCThingObjectDef@@@': 'CTHINGOBJECTDEF',
      '??$GetDef@VCThingBaseDef@@@': 'CTHINGBASEDEF', '??$GetDef@VCExpressionDef@@@': 'CEXPRESSIONDEF'}
OWN = {'CUIMISCTHINGSDEF': ('NUISystem::CUIMiscThingsDef', '0x0042B312'), 'CTHINGOBJECTDEF': ('CThingObjectDef', '0x0042B040'),
       'CTHINGBASEDEF': ('CThingBaseDef', '0x0042AF3C'), 'CEXPRESSIONDEF': ('CExpressionDef', '?')}
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
os.makedirs('%s/var/G' % nf.A, exist_ok=True)
for a in sys.argv[1:]:
    defs = {x[4:] for x in nf.ext_syms(nf.W / 'obj' / ('H_%s.obj' % a)) if x.startswith('DEF ??$GetDef@')}
    types = sorted({SB[p] for d in defs for p in SB if d.startswith(p)}, key=lambda t: list(OWN).index(t))
    other = sorted(d.split('@@@')[0] for d in defs if not any(d.startswith(p) for p in SB))
    L = nf.src_of(a).read_text(encoding='utf-8').split('\n')
    L, rng = drop_template(L)
    k = next(i for i, l in enumerate(L) if l.startswith('#include'))
    ins = gdefs(types)
    L = L[:k] + ins + L[k:]
    open('%s/var/G/%s.cpp' % (nf.A, a), 'w', encoding='utf-8').write('\n'.join(L))
    print(a, 'types=%s' % ','.join(types), 'other_getdef=%s' % (','.join(other) or '-'), 'inserted %d lines above L%d' % (len(ins), k + 1), 'dropped L%d-L%d' % rng)
