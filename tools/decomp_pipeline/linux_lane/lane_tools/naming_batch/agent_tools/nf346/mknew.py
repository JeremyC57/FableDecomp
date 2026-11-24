# mknew.py <addr file>: rows/<addr>.cpp = the landed source with ONE line added right after its
# `#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY` line: the existing CDefPointer.h opt-in FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD.
# Copies the landed source / test to orig/rows / orig/tests (same file names as rows/: <addr>.cpp, <addr>_test.cpp).
import sys, re, shutil
sys.path.insert(0, __file__.rsplit('/', 1)[0]); import nf
LINE = '#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD   // NF346: GetDef<T> copies = retail (the re-reading SetDynamicCast, owner 0x0066C340; as 0x0066B370)'
for a in [l.strip() for l in open(sys.argv[1]) if l.strip()]:
    src = nf.src_of(a); t = src.read_text(encoding='utf-8')
    nl = '\r\n' if '\r\n' in t else '\n'
    L = t.split(nl); k = [i for i, l in enumerate(L) if re.match(r'\s*#\s*define\s+FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY\b', l)]
    assert len(k) == 1 and 'SETDYNAMICCAST_REREAD' not in t, a
    L.insert(k[0] + 1, LINE)
    (nf.A / 'rows' / (a + '.cpp')).write_text(nl.join(L), encoding='utf-8')
    shutil.copy(src, nf.A / 'orig/rows' / (a + '.cpp'))
    tp = nf.test_of(a)
    if tp and tp.exists(): shutil.copy(tp, nf.A / 'orig/tests' / (a + '_test.cpp'))
    print(a, 'line', k[0] + 2, 'crlf' if nl == '\r\n' else 'lf')
