# h30_csm.py: naming wave 30, family csm (scope 1, queue row 24, ~CScriptThing binders): the CTCAIScratchpad script-API rows of the repeated
#  body shape (their landed sibling 0x008A1E50 SetThankingPhrase already has the retail model) re-authored on the shared headers --
#  the invented ScriptMethods projection (??1ScriptMethods@@UAE@XZ bound to 0x004AA840) is engine/CScriptThing.h's retail class (the
#  copy, the destructor and GetPThing expanded as the row's bodies, defined inline after the include), and the own row is the retail
#  virtual member of CGameScriptInterface (FABLE_RETAIL_NSCRIPT, vtable 0x01260F0C) instead of the invented CGameScriptInterfaceMethods.
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
def conv(a):
    p, rel, t = he._rw(a, 'src')
    if 'inline CThing* CScriptThing::GetPThing() const' in t: print('already', a, 'src'); return
    top = '#include "engine/CGameScriptInterface.h"\n#include "engine/CScriptThing.h"\n'
    assert t.count(top) == 1, a; t = t.replace(top, '')
    assert t.count('struct ScriptMethods;\n') == 1, a; t = t.replace('struct ScriptMethods;\n', '')
    t = t.replace('struct CThingNative{', 'class CThing{ public:')
    t = re.sub(r'\bCThingNative\b', 'CThing', t)
    i = t.index('// Complete native CScriptThing virtual interface'); j = t.index('FABLE_STATIC_ASSERT(offsetof(ScriptMethods,PImp)+offsetof(CountedScriptImp,Info)==offsetof(CScriptThing,PImp_Info));\n')
    blk = t[i:j]
    assert '#include "engine/CBaseClass.h"' in blk
    t = t[:i] + '#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0\n' + HDR + t[j + len('FABLE_STATIC_ASSERT(offsetof(ScriptMethods,PImp)+offsetof(CountedScriptImp,Info)==offsetof(CScriptThing,PImp_Info));\n'):]
    m = re.search(r'struct CGameScriptInterfaceMethods:CGameScriptInterface\{[^\n]*?\};', t); assert m, a
    t = t[:m.start()] + t[m.end():]
    t = t.replace('CGameScriptInterfaceMethods::', 'CGameScriptInterface::')
    t = re.sub(r'\bScriptMethods\b', 'CScriptThing', t)
    t = t.replace('.GetThing()', '.GetPThing()')
    he._w(p, t); print('ok', a, 'src')
ROWS = ('008a23b0 008a24c0 008a2590 008a2770 008a2a30 008a3a60 008a3fd0 008a4390 008a4910 008a4ae0 008a5300 008a55c0 008a6750 '
        '008a6a20 008a76f0 008a77f0').split()
LEAF = {}
if os.path.exists(os.path.join(os.path.dirname(os.path.abspath(__file__)), 'csm_leaf.tsv')):
    LEAF = dict(l.rstrip('\n').split('\t') for l in open(os.path.join(os.path.dirname(os.path.abspath(__file__)), 'csm_leaf.tsv')))
for a in ROWS:
    conv(a)
    note(a, 'csm', 'the ScriptMethods projection (??1ScriptMethods@@UAE@XZ bound to 0x004AA840) is engine/CScriptThing.h\'s retail class\n'
         '//   (virtual ~CScriptThing, Ego_r.pdb / FableWin.pdb ??1CScriptThing@@UAE@XZ; copy / dtor / GetPThing slot 11 expanded inline as before),\n'
         '//   CThingNative is the CThing layout, and the own row is the retail virtual CGameScriptInterface member (was the invented\n'
         '//   CGameScriptInterfaceMethods; the landed sibling 0x008A1E50 is the model)', leaf=LEAF.get(a))
# Part 2: the other ScriptMethods script-API rows: the inline bodies follow each row's projection (default ctor, the copy's base form, an
#  inline or out-of-line destructor, GetPThing); CThing::GetScriptThing is the PDB's const ?GetScriptThing@CThing@@QBE?AVCScriptThing@@XZ.
def conv2(a):
    p, rel, t = he._rw(a, 'src')
    if '#include "engine/CScriptThing.h"                         // : CBaseClass' in t: print('already', a, 'src'); return
    top = '#include "engine/CGameScriptInterface.h"\n#include "engine/CScriptThing.h"\n'
    assert t.count(top) == 1, a; t = t.replace(top, '')
    t = t.replace('struct ScriptMethods;\n', '')
    assert 'struct CThingNative{' in t, a
    t = t.replace('struct CThingNative{', 'class CScriptThing;\nclass CThing{ public:')
    t = t.replace('ScriptMethods GetScriptThing();', 'ScriptMethods GetScriptThing() const;')
    t = re.sub(r'\bCThingNative\b', 'CThing', t)
    i = t.index('// Complete native CScriptThing virtual interface') if '// Complete native CScriptThing virtual interface' in t else t.index('// Restored genuine virtual ABI')
    e = 'FABLE_STATIC_ASSERT(offsetof(ScriptMethods,PImp)+offsetof(CountedScriptImp,Info)==offsetof(CScriptThing,PImp_Info));\n'
    j = t.index(e) + len(e); blk = t[i:j]
    inl = []
    if 'ScriptMethods(){}' in blk: inl.append('inline CScriptThing::CScriptThing() { }')
    m = re.search(r'ScriptMethods\(const ScriptMethods& other\):(CBaseClass(?:Native)?)\((other)?\),PImp\(other\.PImp\)\{\}', blk); assert m, a
    inl.append('inline CScriptThing::CScriptThing(const CScriptThing& other) : CBaseClass(%s), PImp(other.PImp) { }' % ('other' if m.group(2) else ''))
    if 'virtual ~ScriptMethods(){}' in blk: inl.append('inline CScriptThing::~CScriptThing() { }')
    else: assert 'virtual ~ScriptMethods();' in blk, a
    if 'GetThing() const {if(!PImp.Data)return 0;return reinterpret_cast<ScriptMethods*>(PImp.Data)->GetPThing();}' in blk:
        inl.append('inline CThing* CScriptThing::GetPThing() const { if (!PImp.Data) return 0; return PImp.Data->GetPThing(); }')
    h = HDR.split("// Retail expands")[0] + "// Retail expands these at this row's sites (the ScriptMethods projection had them in class): defined inline after the header.\n" + \
        ''.join(x + '\n' for x in inl) + '// CGameScriptInterface' + HDR.split('// CGameScriptInterface', 1)[1]
    t = t[:i] + '#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0\n' + h + t[j:]
    m = re.search(r'struct CGameScriptInterfaceMethods:CGameScriptInterface\{[^\n]*?\};', t); assert m, a
    t = t[:m.start()] + t[m.end():]
    t = t.replace('CGameScriptInterfaceMethods::', 'CGameScriptInterface::')
    t = re.sub(r'\bScriptMethods\b', 'CScriptThing', t)
    t = t.replace('.GetThing()', '.GetPThing()')
    he._w(p, t); print('ok', a, 'src')
for a in ('008976f0', '0089faa0'):   # 0x008A8440: deferred (its ScriptVector stand-in is the retail std::vector<CScriptThing>&)
    try: conv2(a)
    except AssertionError as ex: print('FAIL', a, ex); continue
    note(a, 'csm', 'the ScriptMethods projection (??1ScriptMethods@@UAE@XZ bound to 0x004AA840) is engine/CScriptThing.h\'s retail class\n'
         '//   (virtual ~CScriptThing, Ego_r.pdb / FableWin.pdb ??1CScriptThing@@UAE@XZ; the projection\'s inline bodies defined after the include),\n'
         '//   CThingNative is the CThing layout (?GetScriptThing@CThing@@QBE?AVCScriptThing@@XZ, both PDBs: const), and the own row is the retail\n'
         '//   virtual CGameScriptInterface member (was the invented CGameScriptInterfaceMethods)', leaf=LEAF.get(a))
