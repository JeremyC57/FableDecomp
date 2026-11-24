# mkvar362.py <addr> <variant> [<src>]: write rows/var/<addr>_<variant>.cpp from the landed source (or <src>). Variants (combine with '+'):
#   rs   : in-place NF352 opt-in: #define FABLE_RETAIL_CCOUNTEDPOINTER_CCOMPONENT_RESET_SIZE_BODY beside the row's other CCOUNTEDPOINTER defines
#   rd   : in-place NF352 opt-in FABLE_RETAIL_CCOUNTEDPOINTER_CCOMPONENT_RELEASEDATA_SIZE_BODY
#   rr   : in-place #define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD (NF341 / NF349 prerequisite)
#   gu / go : in-place NF349 opt-in GETDEF_CUIMISCTHINGSDEF_SIZE_BODY / GETDEF_CTHINGOBJECTDEF_SIZE_BODY
#   hc   : include move: the row's FABLE_RETAIL_CCOUNTEDPOINTER* / JACKDRAGON_HOT_CTORS_PASS27 defines and `#include "engine/CCountedPointer.h"`
#          (the row's own include line removed if it has one) placed right above the row's first `#pragma optimize`; with rs / rd the
#          opt-in defines go into that hoisted block. Pragmas and every other line are untouched.
import sys, re, os
from pathlib import Path
A = Path('/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad/w22/agentNF364')
sys.path.insert(0, str(A / 'tools')); import nf
a, var = sys.argv[1].lower(), sys.argv[2]; parts = set(var.split('+'))
src = Path(sys.argv[3]) if len(sys.argv) > 3 else nf.src_of(a)
L = src.read_text(encoding='utf-8').split('\n')
first_inc = next(i for i, l in enumerate(L) if l.startswith('#include'))
first_prag = next((i for i, l in enumerate(L) if re.match(r'#pragma optimize', l)), None)
def insert_define(macro):
    # after the last FABLE_RETAIL_CCOUNTEDPOINTER define above the first include, else right above the first include
    k = [i for i, l in enumerate(L[:first_inc]) if re.match(r'#define FABLE_RETAIL_CCOUNTEDPOINTER', l)]
    return (k[-1] + 1) if k else first_inc
adds = []
if 'rd' in parts: adds.append('#define FABLE_RETAIL_CCOUNTEDPOINTER_CCOMPONENT_RELEASEDATA_SIZE_BODY')
if 'rs' in parts: adds.append('#define FABLE_RETAIL_CCOUNTEDPOINTER_CCOMPONENT_RESET_SIZE_BODY')
pre = []
if 'rr' in parts: pre.append('#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD')
if 'gu' in parts: pre.append('#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_CUIMISCTHINGSDEF_SIZE_BODY')
if 'go' in parts: pre.append('#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_CTHINGOBJECTDEF_SIZE_BODY')
if 'hc' in parts:
    assert first_prag is not None
    mv = [l for l in L if re.match(r'#define (FABLE_RETAIL_CCOUNTEDPOINTER\w*|FABLE_RETAIL_JACKDRAGON_HOT_CTORS_PASS27)\s*$', l)]
    L2 = [l for l in L if l not in mv and l.strip() != '#include "engine/CCountedPointer.h"']
    p = next(i for i, l in enumerate(L2) if re.match(r'#pragma optimize', l))
    block = mv + adds + ['#include "engine/CCountedPointer.h"']
    L = L2[:p] + block + L2[p:]
    first_inc = next(i for i, l in enumerate(L) if l.startswith('#include'))
else:
    if adds:
        k = insert_define(None); L = L[:k] + adds + L[k:]; first_inc = next(i for i, l in enumerate(L) if l.startswith('#include'))
if pre:
    # FINAL (2026-11-24): right after the row's last `#define FABLE_RETAIL_CDEFINITIONMANAGER*` / `FABLE_RETAIL_CGAMEDEFINITIONMANAGER*`
    # define above its first include (the NF341 / NF349 rows' position), else just above the row's first #include after the last
    # optimize pragma. `rr` carries a trailing comment naming the copy it repairs (env NF362_RR_NOTE).
    note = os.environ.get('NF362_RR_NOTE')
    if note: pre = [x + '   // ' + note if x.endswith('_SETDYNAMICCAST_REREAD') else x for x in pre]
    dm = [i for i, l in enumerate(L[:first_inc]) if re.match(r'#define FABLE_RETAIL_C(GAME)?DEFINITIONMANAGER\w*\s*(//.*)?$', l)]
    if dm: k = dm[-1] + 1
    else: k = max(first_inc, next((i for i, l in enumerate(L) if l.startswith('#include') and i > (max([j for j, x in enumerate(L) if re.match(r'#pragma optimize', x)] or [0]))), first_inc))
    L = L[:k] + pre + L[k:]
out = (A / 'rows' / (a + '.cpp')) if os.environ.get('NF362_FINAL') else A / 'rows' / 'var' / ('%s_%s.cpp' % (a, var.replace('+', '_'))); out.parent.mkdir(parents=True, exist_ok=True)
out.write_text('\n'.join(L), encoding='utf-8'); print(out)
