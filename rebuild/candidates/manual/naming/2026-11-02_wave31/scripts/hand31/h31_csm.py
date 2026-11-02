# h31_csm.py: naming wave 31, family csm (scope 1, DECISIONS_W30 D2: the four blocked CTCAIScratchpad setters, after family fs): the
#  setters' own retail signature (CScriptThing, CCharString) needs the PDB CCharString copy ctor 0x0099EC30, which fable_string.h now declares
#  (family fs). Re-authored as the wave 30 csm rows (landed sibling 0x008A1E50 model): the ScriptMethods projection is engine/CScriptThing.h's
#  retail class, CThingNative is CThing, the derived NativeCharString view is the header's CCharString, and the own row is the retail virtual
#  ?X@CGameScriptInterface@@UAEXVCScriptThing@@VCCharString@@@Z (was the invented CGameScriptInterfaceMethods over NativeCharString).
#  The scratchpad's inline setter is the PDB CTCAIScratchpad::X(CCharString) by value (FableWin.pdb ?SetAction@CTCAIScratchpad@@QAEXVCCharString@@@Z ...).
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
from common import note
HDR = ('#include "engine/CCountedPointer.h"                     // CCountedPointer<T> for CScriptThing::PImp (header contract)\n'
       '#define FABLE_RETAIL_NSCRIPT\n'
       '#include "engine/CScriptThing.h"                         // : CBaseClass, vtable 0x01238C8C; virtual ~CScriptThing 0x004AA840\n'
       '// Retail expands these at this row\'s sites (the ScriptMethods projection had them in class): defined inline after the header.\n'
       'inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(), PImp(other.PImp) { }\n'
       'inline CScriptThing::~CScriptThing() { }\n'
       'inline CThing* CScriptThing::GetPThing() const { if (!PImp.Data) return 0; return PImp.Data->GetPThing(); }\n'
       '// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:\n'
       '//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).\n'
       '#include "engine/CGameScriptInterface.h"\n')
NCS = ('// Generated fable_string.h lacks the native narrow copy constructor. This layout-compatible\n'
       '// derived view declares its actual out-of-line copy ABI (0099EC30); base assignment0099EFB0 and destructor0099EAE0.\n'
       'struct NativeCharString:CCharString{NativeCharString();NativeCharString(const NativeCharString&);};\n'
       'FABLE_STATIC_ASSERT(sizeof(NativeCharString)==sizeof(CCharString));\n')
def conv(a, fn):
    p, rel, t = he._rw(a, 'src')
    if 'inline CThing* CScriptThing::GetPThing() const' in t: print('already', a, 'src'); return
    top = '#include "engine/CGameScriptInterface.h"\n#include "engine/CScriptThing.h"\n'
    assert t.count(top) == 1, a; t = t.replace(top, '')
    assert t.count('struct ScriptMethods;\n') == 1, a; t = t.replace('struct ScriptMethods;\n', '')
    t = t.replace('struct CThingNative{', 'class CThing{ public:')
    t = re.sub(r'\bCThingNative\b', 'CThing', t)
    i = t.index('// Complete native CScriptThing virtual interface'); e = 'FABLE_STATIC_ASSERT(offsetof(ScriptMethods,PImp)+offsetof(CountedScriptImp,Info)==offsetof(CScriptThing,PImp_Info));\n'
    j = t.index(e)
    assert '#include "engine/CBaseClass.h"' in t[i:j]
    t = t[:i] + '#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0\n' + HDR + t[j + len(e):]
    assert t.count(NCS) == 1, a; t = t.replace('\n' + NCS, '\n') if ('\n' + NCS) in t else t.replace(NCS, '')
    m = re.search(r'struct CGameScriptInterfaceMethods:CGameScriptInterface\{[^\n]*?\};\n', t); assert m, a
    t = t[:m.start()] + t[m.end():]
    t = t.replace('CGameScriptInterfaceMethods::', 'CGameScriptInterface::')
    t = re.sub(r'\bScriptMethods\b', 'CScriptThing', t)
    t = re.sub(r'\bNativeCharString\b', 'CCharString', t)
    t = t.replace('.GetThing()', '.GetPThing()')
    assert 'void CGameScriptInterface::%s(CScriptThing script,CCharString text)' % fn in t, a
    assert 'void %s(CCharString text){' % fn in t, a
    he._w(p, t); print('ok', a, 'src')
ROWS = {'008a41b0': 'SetAction', '008a4730': 'SetTargetName', '008a6570': 'SetTableName', '008a6840': 'SetSeatName'}
for a, fn in sorted(ROWS.items()):
    conv(a, fn)
    note(a, 'csm', 'the CCharString parameter is the header\'s class with the PDB copy ctor ??0CCharString@@QAE@ABV0@@Z 0x0099EC30 (family fs;\n'
         '//   was the derived NativeCharString view), the ScriptMethods projection is engine/CScriptThing.h\'s retail class (virtual ~CScriptThing,\n'
         '//   ??1CScriptThing@@UAE@XZ 0x004AA840; copy / dtor / GetPThing expanded inline as before), CThingNative is CThing, and the own row is the\n'
         '//   retail virtual CGameScriptInterface member (was the invented CGameScriptInterfaceMethods; the landed sibling 0x008A1E50 is the model);\n'
         '//   the scratchpad setter is the PDB CTCAIScratchpad::%s(CCharString) by value (FableWin.pdb), expanded inline' % fn,
         leaf='?%s@CGameScriptInterface@@UAEXVCScriptThing@@VCCharString@@@Z' % fn)
