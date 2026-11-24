# mkclone.py <addr> <Class> <extra defines comma-separated or -> [header-name]: Clone-family draft (NF323, 2026-11-24).
# Keeps the HEAD row's leading comment block (minus the `// Retail bytes:` line is KEPT verbatim), drops the local stand-in models,
# adds the full PDB Leaf, a provenance note, the class-model switches and the shared headers (base first), and defines
# `CCreatureActionBase* <Class>::Clone() const` with HEAD's body. Saves orig/<addr>.cpp. -> rows/<addr>.cpp
import sys, re, shutil
sys.path.insert(0, '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF336/tools'); import nf
A = '/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF336'
a, C, ex = sys.argv[1], sys.argv[2], sys.argv[3]
hdr = sys.argv[4] if len(sys.argv) > 4 else C
src = nf.src_of(a); t = open(src, encoding='utf-8').read()
shutil.copy(src, A + '/orig/%s.cpp' % a)
tp = nf.test_of(a)
if tp: shutil.copy(tp, A + '/orig/%s_test.cpp' % a)
lines = t.split('\n'); head = []
for l in lines:
    if l.startswith('//'): head.append(l)
    else: break
m = re.search(r'^(CCreatureActionBase\* %s::Clone\(\)[^\n]*)\n(\{.*?\n\})' % re.escape(C), t, re.M | re.S)
body = m.group(2); oldhead = m.group(1)
old = re.search(r'\bCCreatureActionBase\* Clone\(\)( const)?;', t)
oldsym = '?Clone@%s@@Q%sEPAVCCreatureActionBase@@XZ' % (C, 'B' if (old and old.group(1)) else 'A')
defs = ['FABLE_RETAIL_BASE_OBJECT', 'FABLE_RETAIL_CCREATUREACTIONBASE', 'FABLE_RETAIL_CCREATUREACTIONBASE_COPYCTOR'] + [x for x in ex.split(',') if x and x != '-']
out = head + [
    '// Leaf: ?Clone@%s@@UBEPAVCCreatureActionBase@@XZ' % C,
    '// Clone port (NF323, 2026-11-24; NF317 Q3 / NF320 Q1): defined on the real class of engine/%s.h over engine/CCreatureActionBase.h' % hdr,
    '//   instead of the local one-slot stand-in model (was %s). The inline implicit copy constructor stores' % oldsym,
    '//   the class vftable, so this TU emits ??_7%s@@6B@ from the header model, slot for slot the retail table.' % C,
] + ['#define ' + d for d in defs] + ['#include "engine/CCreatureActionBase.h"', '#include "engine/%s.h"' % hdr, '',
    'CCreatureActionBase* %s::Clone() const' % C, body, '']
open(A + '/rows/%s.cpp' % a, 'w', encoding='utf-8').write('\n'.join(out))
print(a, C, oldsym)
