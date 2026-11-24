# mknew349.py <addr file>: from the landed source (repo HEAD) build, per row:
#   rows/<a>.cpp       N: HEAD + the REREAD define + one size-body opt-in define per size-mode GetDef<T> type the row emits
#   work/var/<a>_R.cpp R: HEAD + the REREAD define only (NF346's measurement text, rebuilt)
#   work/var/<a>_W.cpp W: R + NF349_WRAP_TEST (the brief's literal text: the template wrapped in the pragma pair, overlay work/hdrW)
# Lines go right after the row's `#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY`. Types from work/types40.tsv (NF346 recount).
# Copies the landed source / test to orig/rows, orig/tests.
import sys, re, shutil
sys.path.insert(0, __file__.rsplit('/', 1)[0]); import nf
RR = '#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD   // NF349: GetDef<T> copies = retail (the re-reading SetDynamicCast, owner 0x0066C340; as 0x0066B370)'
OPT = {'0042af3c': ('CTHINGBASEDEF', 'CThingBaseDef', '0x0042AF3C'), '0042b040': ('CTHINGOBJECTDEF', 'CThingObjectDef', '0x0042B040'),
       '0042b312': ('CUIMISCTHINGSDEF', 'CUIMiscThingsDef', '0x0042B312'), '005f81be': ('CEXPRESSIONDEF', 'CExpressionDef', '0x005F81BE')}
TY = {l.split('\t')[0]: l.rstrip('\n').split('\t')[1].split() for l in open(nf.A / 'work/types40.tsv')}
for a in [l.strip() for l in open(sys.argv[1]) if l.strip()]:
    src = nf.src_of(a); t = src.read_text(encoding='utf-8')
    nl = '\r\n' if '\r\n' in t else '\n'
    L = t.split(nl); k = [i for i, l in enumerate(L) if re.match(r'\s*#\s*define\s+FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY\b', l)]
    assert len(k) == 1 and 'SETDYNAMICCAST_REREAD' not in t and not re.search(r'GETDEF_\w*SIZE_BODY', t), a
    ops = []
    for c in TY.get(a, []):
        o = c.split(':')[0]
        if o in OPT:
            u, n, ad = OPT[o]
            ops.append('#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_%s_SIZE_BODY   // NF349: GetDef<%s> pick-any copy = size-mode owner %s (scaffolding, not retail)' % (u, n, ad))
    assert ops, a
    def mk(extra): M = list(L); M[k[0] + 1:k[0] + 1] = [RR] + extra; return nl.join(M)
    (nf.A / 'rows' / (a + '.cpp')).write_text(mk(ops), encoding='utf-8')
    (nf.A / 'work/var' / (a + '_R.cpp')).write_text(mk([]), encoding='utf-8')
    (nf.A / 'work/var' / (a + '_W.cpp')).write_text(mk(['#define NF349_WRAP_TEST   // NF349 measurement only: the literal pragma wrap of the template']), encoding='utf-8')
    shutil.copy(src, nf.A / 'orig/rows' / (a + '.cpp'))
    tp = nf.test_of(a)
    if tp and tp.exists(): shutil.copy(tp, nf.A / 'orig/tests' / (a + '_test.cpp'))
    print(a, 'line', k[0] + 2, 'crlf' if nl == '\r\n' else 'lf', len(ops), 'opt-ins')
