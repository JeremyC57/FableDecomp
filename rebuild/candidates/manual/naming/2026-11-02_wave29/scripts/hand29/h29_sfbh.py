# h29_sfbh.py: naming wave 29, family sfbh (scope 1; DECISIONS_W28 D5, chunk 696 decisions 2 / 3) -- engine/CSpawnedFuncBase.h's
#  retail branch (FABLE_RETAIL_SUSPENDABLE_PROCESS) gets retail's implicit destructor and Ego_r.pdb's real member types
#  (CCharString Name / AssociatedRegionName, CCountedPointer<const NScript::CPredicateBase> PPredicate). The header follows the
#  established contract of engine/CEntityScriptBinding.h / CActiveEntityScriptBase.h: the includer defines CCharString and the
#  CCountedPointer<T> template first. Includers that did not (runs/sfbh_fixes.tsv) get, before their first CSpawnedFunc* header:
#   cp    #include "engine/CCountedPointer.h"
#   cs    #include "fable_string.h"
#   local a declaration-only CCountedPointer<T> (rows with their own CCPPointerInfo, which engine/CCountedPointer.h would redefine)
#   move  the row's own CCountedPointer<T> moved above the include
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
from common import note
HDR = os.path.join(he.W, 'scripts', 'hand29', 'CSpawnedFuncBase.h.new')
dst = os.path.join(he.W, os.environ.get('HE_OV', 'ov_hand'), 'rebuild/include/engine/CSpawnedFuncBase.h')
os.makedirs(os.path.dirname(dst), exist_ok=True)
if not os.path.exists(dst) or open(dst, newline='').read() != open(HDR, newline='').read():
    open(dst, 'w', newline='').write(open(HDR, newline='').read()); print('ok header')
else: print('already header')
HX = re.compile(r'^#include "engine/(CSpawnedFuncBase|CSpawnedFunc|CSpawnedFunc1|CActiveEntityScriptBase|CActiveEntityScript)\.h".*$', re.M)
L_CP = '#include "engine/CCountedPointer.h"                     // CCountedPointer<T> for CSpawnedFuncBase::PPredicate (header contract)\n'
L_CS = '#include "fable_string.h"                               // CCharString for CSpawnedFuncBase::Name / AssociatedRegionName (header contract)\n'
L_LOC = ('template <class T> class CCountedPointer {             // CSpawnedFuncBase::PPredicate (header contract; this row keeps its own\n'
         'public:                                                //   CCPPointerInfo, so not engine/CCountedPointer.h); the dtor is out of line\n'
         '    T* Data; CCPPointerInfo* Info;                     //   in retail (CCountedPointer<const NScript::CPredicateBase> 0x00CDD4F0)\n'
         '    ~CCountedPointer();\n'
         '};\n')
WHY = {'cp': 'engine/CCountedPointer.h included before it', 'cs': 'fable_string.h (CCharString) included before it',
       'cp,cs': 'engine/CCountedPointer.h and fable_string.h included before it', 'local': 'a declaration-only CCountedPointer<T> declared before it (the row keeps its own CCPPointerInfo)',
       'move': "the row's own CCountedPointer<T> moved above it"}
for l in open(os.path.join(he.W, 'runs', 'sfbh_fixes.tsv')):
    a, k = l.split()
    p, rel, t = he._rw(a, 'src')
    if 'family sfbh' in t: print('already', a); continue
    m = HX.search(t)
    if k == 'move':
        mm = re.search(r'^template <class T> class CCountedPointer \{[^\n]*\};\n', t, re.M)
        assert mm and mm.start() > m.start(), a
        line = mm.group(0); t = t[:mm.start()] + t[mm.end():]; m = HX.search(t); ins = line
    else:
        ins = {'cp': L_CP, 'cs': L_CS, 'cp,cs': L_CP + L_CS, 'local': L_LOC}[k]
    t = t[:m.start()] + ins + t[m.start():]
    he._w(p, t); print('ok', a, 'src')
    note(a, 'sfbh', 'engine/CSpawnedFuncBase.h now carries retail\'s implicit destructor and the real CCharString /\n'
         '//   CCountedPointer<const NScript::CPredicateBase> members; header contract met here: %s.' % WHY[k])
