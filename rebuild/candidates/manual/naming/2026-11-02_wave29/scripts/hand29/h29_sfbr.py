# h29_sfbr.py: naming wave 29, family sfbr (scope 1; chunk 696 decision 2): the 14 sources that modelled the spawned-function base as
#  an invented CScriptProcessBaseRuntime / CSpawnedFuncBaseRuntime pair (virtual ~CSpawnedFuncBaseRuntime, RuntimeHookCDD410 /
#  RuntimeHookCDD420 defined in-TU) derive their CSpawnedFunc1-shaped thread class from the retail CSpawnedFuncBase of
#  engine/CSpawnedFuncBase.h (FABLE_RETAIL_SUSPENDABLE_PROCESS: : CSuspendableProcessMicrothread, ctor 0x00CDD450, implicit dtor
#  0x00CDD4C0, slots 5 / 6 OnPredicateFail 0x00CDD410 / OnInterrupted 0x00CDD420). The flattened engine/CSpawnedFuncBase.h /
#  CASuspendableProcess.h includes these rows carried unused are dropped (the retail chain includes CASuspendableProcess).
import os, sys, re, json
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
from common import note
ROWS = sorted(json.loads(l)['a'] for l in open(os.path.join(he.W, 'idx', 'full.jsonl')) if '??1CSpawnedFuncBaseRuntime@@UAE@XZ' in l)
L_LOC = ('template <class T> class CCountedPointer {             // CSpawnedFuncBase::PPredicate (header contract; this row keeps its own\n'
         'public:                                                //   CCPPointerInfo, so not engine/CCountedPointer.h); the dtor is out of line\n'
         '    T* Data; CCPPointerInfo* Info;                     //   in retail (CCountedPointer<const NScript::CPredicateBase> 0x00CDD4F0)\n'
         '    ~CCountedPointer();\n'
         '};\n')
INC = ('// CSpawnedFuncBase: the retail model of engine/CSpawnedFuncBase.h (FABLE_RETAIL_SUSPENDABLE_PROCESS: : CSuspendableProcessMicrothread\n'
       '//   : CASuspendableProcess : CBaseClassNonCopyable : CBaseClass, 0x34, vtable 0x012C2F9C; ctor 0x00CDD450, implicit dtor 0x00CDD4C0).\n'
       '#define FABLE_RETAIL_SUSPENDABLE_PROCESS\n'
       '#include "engine/CSpawnedFuncBase.h"')
for a in ROWS:
    p, rel, t = he._rw(a, 'src')
    if 'family sfbr' in t: print('already', a); continue
    i = t.index('struct CScriptProcessBaseRuntime {')
    j = t.index('\n};', t.index('struct CSpawnedFuncBaseRuntime')) + 3
    pre = t[:i]
    ins = INC if re.search(r'(class|struct)\s+CCountedPointer\b[^;{]*\{', pre) else L_LOC + INC
    t = pre + ins + t[j:]
    t = re.sub(r'\nvoid CScriptProcessBaseRuntime::RuntimeHookCDD4[12]0\(\) \{\}', '', t)
    t = re.sub(r'#include "engine/CSpawnedFuncBase\.h"\n(?!.*retail)', '', t, count=1) if t.count('#include "engine/CSpawnedFuncBase.h"') > 1 else t
    t = t.replace('#include "engine/CASuspendableProcess.h"\n', '')
    t = t.replace('CSpawnedFuncBaseRuntime', 'CSpawnedFuncBase')
    if 'CScriptProcessBaseRuntime' in t: raise SystemExit('left CScriptProcessBaseRuntime in ' + rel)
    he._w(p, t); print('ok', a, 'src')
    note(a, 'sfbr', 'the thread class derives from the retail CSpawnedFuncBase (engine/CSpawnedFuncBase.h; Ego_r.pdb / FableWin.pdb\n'
         '//   ??0CSpawnedFuncBase@@QAE@ABVCCharString@@K@Z 0x00CDD450, implicit ??1CSpawnedFuncBase@@UAE@XZ 0x00CDD4C0). Was the invented\n'
         '//   CScriptProcessBaseRuntime / CSpawnedFuncBaseRuntime pair (virtual ~CSpawnedFuncBaseRuntime, RuntimeHookCDD410 / CDD420).')
