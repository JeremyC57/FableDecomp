# h17_c677.py: naming wave 17, the c677 queue rows that respell landed sources by hand (name-only; the recheck proves the bytes):
#  0x00880A40 NScreenEffect::CScreenEffectPriorityStack::Reset is private (Ego_r/FableWin ?Reset@...@AAEXXZ);
#  0x00881370 the ctor (and its vtable 0x01278058 ??_7CScreenEffectPriorityStack@NScreenEffect@@6B@) lives in NScreenEffect and calls
#             the private Reset;
#  0x00D6EF20 NScript::CQ_HangingTreeGoodScript's ctor on the PDB (CScriptDataBase*, CGameScriptInterfaceBase&) pair (wave 16 qctor);
#  0x00CFA2F0 the CQ_AmbushTradersScript factory is ?Alloc@CQ_AmbushTradersScript@NScript@@SIPAVCScriptBase@@... (Ego_r lists it),
#             landed as the free Create_CClass_00cf9350_00cfa2f0 (the wave 16 Alloc shape; base as the landed ctor 0x00CF9350 models it).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit, _rw, _w
edit('00880a40', 'src', '''class CScreenEffectPriorityStack {
public:
    void Reset();
private:''', '''class CScreenEffectPriorityStack {
private:                                    // Ego_r / FableWin ?Reset@CScreenEffectPriorityStack@NScreenEffect@@AAEXXZ (naming wave 17, c677)
    void Reset();''')
p, rel, t = _rw('00881370', 'src')
old = 'struct CScreenEffectPriorityStack {virtual ~CScreenEffectPriorityStack();List<104> a[3];Tree ta;char gap1[0x50];List<40>b[3];Tree tb;char gap2[0x10];List<68>c[3];Tree tc;CScreenFilterParams CurrentScreenFilterParam;CScreenEffectPriorityStack();void Reset();};\nCScreenEffectPriorityStack::CScreenEffectPriorityStack(){Reset();}'
if old in t:
    t = t.replace(old, '''// Naming wave 17 (c677): the class is NScreenEffect::CScreenEffectPriorityStack (Ego_r/FableWin member spellings; vtable 0x01278058
//   ??_7CScreenEffectPriorityStack@NScreenEffect@@6B@) and Reset is private (?Reset@...@AAEXXZ); name-only.
namespace NScreenEffect {
class CScreenEffectPriorityStack {public:virtual ~CScreenEffectPriorityStack();List<104> a[3];Tree ta;char gap1[0x50];List<40>b[3];Tree tb;char gap2[0x10];List<68>c[3];Tree tc;CScreenFilterParams CurrentScreenFilterParam;CScreenEffectPriorityStack();private:void Reset();};
}
NScreenEffect::CScreenEffectPriorityStack::CScreenEffectPriorityStack(){Reset();}''')
    t = t.replace('// Leaf: ??0CScreenEffectPriorityStack\n', '// Leaf: ??0CScreenEffectPriorityStack@NScreenEffect@@QAE@XZ\n')
    _w(p, t); print('ok 00881370 src')
else: print('already 00881370')
a = '00d6ef20'
P = 'CScriptDataBase* data, CGameScriptInterfaceBase& game'
edit(a, 'src', '// CQ_HangingTreeGoodScript::CQ_HangingTreeGoodScript(void* owner, CGameScriptInterface* game) -- quest-script constructor: script base, owner/interface pair,',
     '// NScript::CQ_HangingTreeGoodScript::CQ_HangingTreeGoodScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- quest-script constructor: script base, data/interface pair,')
edit(a, 'src', '// Leaf: ??0CQ_HangingTreeGoodScript\n', '''// Leaf: ??0CQ_HangingTreeGoodScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Naming wave 17 (c677): the PDB parameter pair (CScriptDataBase*, CGameScriptInterfaceBase&) replaces the landed (void*,
//   CGameScriptInterface*); name-only (both are one pushed pointer: the bytes are unchanged). Only caller Alloc 0x00D78290.
''')
edit(a, 'src', 'class CGameScriptInterface;\n', 'class CGameScriptInterfaceBase;\nclass CScriptDataBase;\n')
edit(a, 'src', '''    CGameScriptInterface* m_pGame;                   // +0x40
    void* m_pOwner;                                  // +0x44
    CScriptObject(void* owner, CGameScriptInterface* game) : CScriptBase(), m_pGame(game), m_pOwner(owner) { }''',
     '''    CGameScriptInterfaceBase* m_pGame;               // +0x40 (Ego_r CLevelScriptBase<T>::Game)
    CScriptDataBase* m_pOwner;                       // +0x44 (the script data)
    CScriptObject(%s) : CScriptBase(), m_pGame(&game), m_pOwner(data) { }''' % P)
edit(a, 'src', '''    CQ_HangingTreeGoodScript(void* owner, CGameScriptInterface* game);''', '''    CQ_HangingTreeGoodScript(%s);''' % P)
edit(a, 'src', '''NScript::CQ_HangingTreeGoodScript::CQ_HangingTreeGoodScript(void* owner, CGameScriptInterface* game)
    : CScriptObject(owner, game)''', '''NScript::CQ_HangingTreeGoodScript::CQ_HangingTreeGoodScript(%s)
    : CScriptObject(data, game)''' % P)
p, rel, t = _rw('00cfa2f0', 'src')
if 'Create_CClass' in t:
    _w(p, '''// NScript::CQ_AmbushTradersScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new CQ_AmbushTradersScript(data, game)
// Leaf: ?Alloc@CQ_AmbushTradersScript@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CFA2F0, 40 bytes, __fastcall (data in ecx, the interface in edx).
// Identity (proven, chunk c677): new(0x1D0) + ctor 0x00CF9350 (??0CQ_AmbushTradersScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z,
//   its own row), registered as Q_AmbushTraders (S_QAT); Ego_r.pdb lists this exact symbol (ego_r.exe.signatures.tsv 0x00F8E7E0).
// Callees: operator new 0x00BFEA1A, CQ_AmbushTradersScript::CQ_AmbushTradersScript 0x00CF9350.
// Shape: retail bytes push esi ; push edi ; push 0x1d0 ; mov esi,edx ; mov edi,ecx ; call operator new ; add esp,4 ; test eax,eax ;
//   je N ; push esi ; push edi ; mov ecx,eax ; call ctor ; pop edi ; pop esi ; ret ; N: pop edi ; xor eax,eax ; pop esi ; ret
// Naming wave 17: landed as the free __fastcall Create_CClass_00cf9350_00cfa2f0(void*, void*) on a CClass_00cf9350 stand-in;
//   respelled on the PDB static member (name-only: the bytes are unchanged). Base as the landed ctor row 0x00CF9350 models it.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_AmbushTradersScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CQ_AmbushTradersScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);   // 0x00CF9350
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    unsigned char _members[0x1d0 - 0x48];                // +0x48 (the script's own members; retail sizeof 0x1D0)
};
CScriptBase* __fastcall CQ_AmbushTradersScript::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new CQ_AmbushTradersScript(data, game);
}
}
typedef char ExactOwnerSize[sizeof(NScript::CQ_AmbushTradersScript) == 0x1d0 ? 1 : -1];
'''); print('ok 00cfa2f0 src')
else: print('already 00cfa2f0')
# 0x006B8410 forwards (add ecx,0x90 ; jmp) to NScreenEffect::CScreenEffectPriorityStack::Clear 0x00881210 (c677: Ego_r/FableWin Clear,
#   the only no-arg non-accessor member) on its +0x90 member; the landed CBaseIntelligentPointer::Method_00881210 stand-in is that call.
#   The row's own descriptive name (GetTarget_006b8410) is kept; it now returns void as Clear does (same bytes: a sibling jump).
edit('006b8410', 'src', '''struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;
    CBaseObject* Method_00881210();               // 0x00881210
};''', '''namespace NScreenEffect {
class CScreenEffectPriorityStack {                // Ego_r/FableWin (naming wave 17, c677: was CBaseIntelligentPointer::Method_00881210)
public:
    void Clear();                                 // 0x00881210 (?Clear@CScreenEffectPriorityStack@NScreenEffect@@QAEXXZ)
};
}''')
edit('006b8410', 'src', '''    CBaseIntelligentPointer m_pointer;     // +0x90
    virtual CBaseObject* GetTarget_006b8410();
};

CBaseObject* CClass_006b8410::GetTarget_006b8410()
{
    return m_pointer.Method_00881210();
}''', '''    NScreenEffect::CScreenEffectPriorityStack m_screenEffectStack;   // +0x90
    virtual void GetTarget_006b8410();
};

void CClass_006b8410::GetTarget_006b8410()
{
    m_screenEffectStack.Clear();
}''')
edit('006b8410', 'src', '// CClass_006b8410::GetTarget_006b8410() -- returns m_pointer.Method_00881210() for the smart pointer at +0x90 (sibling jump)',
     '// CClass_006b8410::GetTarget_006b8410() -- forwards to NScreenEffect::CScreenEffectPriorityStack::Clear() on the member at +0x90 (sibling jump)')
edit('00883e50', 'src', '''    void _M_erase(_Link_type __x);                        // 0x00882BC0
''', '''private:
    void _M_erase(_Link_type __x);                        // 0x00882BC0 (STLport: private, AAE -- naming wave 17, c677)
public:
''')
