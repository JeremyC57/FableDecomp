# mkrow364.py <addr...>: write rows/<addr>.cpp = the landed source plus ONE line, `#define FABLE_RETAIL_CCOUNTEDPOINTER_CCOMPONENT_RESET_SIZE_BODY_YOFF_TU`
# (with a `// NF364:` note), placed after the row's last `#define FABLE_RETAIL_CCOUNTEDPOINTER*` line above its first #include (else right above
# the first #include). Asserts: exactly one added line, above the first #include; the row defines none of the conflicting macros
# (RESET_SIZE_BODY, RELEASEDATA_SIZE_BODY, ORIGINAL_COMPLETE_MEMBERS_PASS22); copies the landed source to orig/rows/ and the test to orig/tests/.
import sys, re, difflib, shutil
from pathlib import Path
A = Path('/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF364')
sys.path.insert(0, str(A / 'tools')); import nf
LINE = '#define FABLE_RETAIL_CCOUNTEDPOINTER_CCOMPONENT_RESET_SIZE_BODY_YOFF_TU   // NF364: Reset<CComponent> 0x00429C6F pick-any copy in this y-off / s-on TU'
BAD = ('FABLE_RETAIL_CCOUNTEDPOINTER_CCOMPONENT_RESET_SIZE_BODY', 'FABLE_RETAIL_CCOUNTEDPOINTER_CCOMPONENT_RELEASEDATA_SIZE_BODY', 'FABLE_RETAIL_CCOUNTEDPOINTER_ORIGINAL_COMPLETE_MEMBERS_PASS22')
for a in sys.argv[1:]:
    sp = nf.src_of(a); tp = nf.test_of(a); t = sp.read_text(encoding='utf-8'); L = t.split('\n')
    for b in BAD: assert not re.search(r'#\s*define\s+%s\b' % b, t), (a, b)
    fi = next(i for i, l in enumerate(L) if l.startswith('#include'))
    k = [i for i, l in enumerate(L[:fi]) if re.match(r'#define FABLE_RETAIL_CCOUNTEDPOINTER', l)]
    pos = (k[-1] + 1) if k else fi
    N = L[:pos] + [LINE] + L[pos:]
    d = [x for x in difflib.unified_diff(L, N, lineterm='', n=0) if x[:1] in '+-' and x[:3] not in ('+++', '---')]
    assert d == ['+' + LINE], (a, d)
    assert pos <= next(i for i, l in enumerate(N) if l.startswith('#include'))
    (A / 'rows').mkdir(exist_ok=True); (A / 'rows' / (a + '.cpp')).write_text('\n'.join(N), encoding='utf-8')
    (A / 'orig/rows').mkdir(parents=True, exist_ok=True); (A / 'orig/tests').mkdir(parents=True, exist_ok=True)
    shutil.copy(sp, A / 'orig/rows' / (a + '.cpp')); shutil.copy(tp, A / 'orig/tests' / (a + '_test.cpp'))
    print(a, 'line', pos + 1, 'after', (L[pos - 1][:60] if pos else '-'), 'REREAD' if 'SETDYNAMICCAST_REREAD' in t else '')
