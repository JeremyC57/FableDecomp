# h18_qctor.py: naming wave 18, family qctor -- the c678 / c681 quest-script constructor own rows re-authored name-only on the PDB
# constructor ??0<C>@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z (h17_qctor pattern: the (CScriptDataBase*,
# CGameScriptInterfaceBase&) pair is one pushed pointer each, Game is the Ego_r CLevelScriptBase<T>::Game member), the unnested
# classes moved into NScript; the nested CV_FishingCompetitionScript::CFishCompOrganiser ctor on its PDB signature
# (AAV12@ = the owning CV_FishingCompetitionScript&); the two Create_* Alloc stand-ins on ?Alloc@<C>@NScript@@SIPAVCScriptBase@@...
# (as 0x00CFA2F0, chunk c677). The tests are self-contained (they define their own model) and are unchanged.
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
from he import edit, resub, put, _rw, _w
P = 'CScriptDataBase* data, CGameScriptInterfaceBase& game'
NOTE = '// Naming wave 18: landed with the (void*, CGameScriptInterface*) parameter pair%s; respelled on the PDB constructor\n//   (CScriptDataBase*, CGameScriptInterfaceBase&) -- name-only (one pushed pointer each, Game = Ego_r CLevelScriptBase<T>::Game).\n'
def common(a, cls, leaf_old, base='CQuestScriptObject', unnested=False, extra=''):
    p, rel, t = _rw(a, 'src')
    if 'Naming wave 18' in t: print('already', a); return
    sym = '??0%s@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z' % cls
    t = t.replace('// Leaf: ' + leaf_old + '\n', '// Leaf: %s\n' % sym + NOTE % extra, 1)
    assert sym in t, a
    # header line 1
    L = t.split('\n')
    L[0] = L[0].replace('(void* data, CGameScriptInterface* game)', '(%s)' % P).replace('(void* owner, CGameScriptInterface* game)', '(%s)' % P).replace('(CScriptDataBase*, CGameScriptInterface&)', '(%s)' % P)
    if not L[0].startswith('// NScript::'): L[0] = '// NScript::' + L[0][3:]
    t = '\n'.join(L)
    # intermediate base
    t, n = re.subn(r'CGameScriptInterface\*(\s+)(Game|m_pGame|GSI);', r'CGameScriptInterfaceBase*\1\2;', t); assert n == 1, (a, 'Game', n)
    t, n = re.subn(r'void\*(\s+)(Data|m_pOwner);', r'CScriptDataBase*\1\2;', t); assert n <= 1, (a, 'Data', n)
    t, n = re.subn(base + r'\(void\* (data|owner), CGameScriptInterface\* game\) : CScriptBase\(\), (\w+)\(game\), (\w+)\((data|owner)\)',
                   base + r'(%s) : CScriptBase(), \2(&game), \3(data)' % P, t)
    if n == 0: t, n = re.subn(base + r'\(CScriptDataBase\* data, CGameScriptInterface\* gsi\) : CScriptBase\(\), GSI\(gsi\), Data\(data\)', base + r'(%s) : CScriptBase(), GSI(&game), Data(data)' % P, t)
    assert n == 1, (a, 'base ctor', n)
    # the class ctor declaration + definition
    t, n = re.subn(r'\b' + cls + r'\((void\* (data|owner), CGameScriptInterface\* game|CScriptDataBase\* data, CGameScriptInterface& gsi)\)', cls + '(%s)' % P, t); assert n in (2, 3), (a, 'ctor', n)
    t = t.replace(': %s(owner, game)' % base, ': %s(data, game)' % base).replace(': %s(data, &gsi)' % base, ': %s(data, game)' % base)
    # forward declarations
    if 'class CScriptDataBase;' not in t:
        t = re.sub(r'(?m)^(struct __declspec\(novtable\) ' + base + r'\b)', 'class CScriptDataBase;\n\\1', t, count=1)
    if 'CGameScriptInterfaceBase.h' not in t and 'CGameScriptInterface.h' not in t and 'class CGameScriptInterfaceBase;' not in t:
        t = t.replace('class CGameScriptInterface;\n', 'class CGameScriptInterface;\nclass CGameScriptInterfaceBase;\n', 1)
    assert 'class CScriptDataBase;' in t, a
    _w(p, t); print('ok', a, 'src')
    if unnested:
        he.nscript(a, 'src', cls)
        p, rel, t = _rw(a, 'src')
        t, n = re.subn(r'(?m)^' + cls + '::' + cls + r'\(', 'NScript::' + cls + '::' + cls + '(', t); assert n == 1, (a, 'def', n)
        _w(p, t)
common('00e1db00', 'CQR_EscortTraderScript', '??0CQR_EscortTraderScript', base='CScriptObject', unnested=True, extra=' (unnested; the members m_pGame / m_pOwner)')
common('00e755f0', 'CV_ExposeMayorScript', '??0CV_ExposeMayorScript')
common('00d9e210', 'CQ_MinionCampScript', '??0CQ_MinionCampScript')
common('00cf88c0', 'CLadyGreyWifeManagerScript', '??0CLadyGreyWifeManagerScript', unnested=True, extra=' (unnested)')
common('00de0320', 'CQ_RansomVictimScript', '??0CQ_RansomVictimScript', base='CQuestScriptBase', extra=' as (CScriptDataBase*, CGameScriptInterface&)')
common('00e398f0', 'CV_BordelloScript', '??0CV_BordelloScript')
common('00dec7f0', 'CQ_SecretPassageScript', '??0CQ_SecretPassageScript', extra=' (chunk c678)')
# the nested CFishCompOrganiser ctor
a = '00e7ef70'
p, rel, t = _rw(a, 'src')
if 'Naming wave 18' not in t:
    t = t.replace('// CFishCompOrganiser::CFishCompOrganiser(CScriptOwner* parent, void* masterData, const CScriptThing& me)',
                  '// NScript::CV_FishingCompetitionScript::CFishCompOrganiser::CFishCompOrganiser(CV_FishingCompetitionScript& parent, CScriptDataBase* masterData, const CScriptThing& me)', 1)
    t = t.replace('// Leaf: ??0CFishCompOrganiser\n', '// Leaf: ??0CFishCompOrganiser@CV_FishingCompetitionScript@NScript@@QAE@AAV12@PAVCScriptDataBase@@ABVCScriptThing@@@Z\n'
                  '// Naming wave 18: landed unnested as CFishCompOrganiser(CScriptOwner*, void*, const CScriptThing&); respelled on the PDB constructor of the\n'
                  '//   nested class (Ego_r / FableWin ??0CFishCompOrganiser@CV_FishingCompetitionScript@NScript@@QAE@AAV12@PAVCScriptDataBase@@ABVCScriptThing@@@Z:\n'
                  '//   the owning CV_FishingCompetitionScript by reference, the master data a CScriptDataBase*) -- name-only (the bytes are unchanged).\n', 1)
    t = t.replace('''struct CScriptOwner {                         // the quest script that owns this entity script
    unsigned char _pad00[0x40];
    void*         m_pInterface;               // +0x40 CGameScriptInterfaceBase*
};
class CEntityScriptBase { public:                    // vtable 0x012C3224, 0x14 bytes
    virtual void v00();
    void*        Game;                        // +0x04 (a plain initialiser: stored before the vtable)
    CScriptThing Me;                          // +0x08
    CEntityScriptBase(const CScriptThing& thing, void* game) : Game(game), Me(thing) { }
};

class CFishCompOrganiser : public CEntityScriptBase {   // vtable 0x012EA620
public:
    CScriptOwner* ParentClass;                // +0x14 CV_FishingCompetitionScript*
    void*         PMasterData;                // +0x18 CQ_SunnyvaleMasterData*''',
'''class CGameScriptInterfaceBase;
class CScriptDataBase;
class CEntityScriptBase { public:                    // vtable 0x012C3224, 0x14 bytes
    virtual void v00();
    CGameScriptInterfaceBase* Game;           // +0x04 (a plain initialiser: stored before the vtable)
    CScriptThing Me;                          // +0x08
    CEntityScriptBase(const CScriptThing& thing, CGameScriptInterfaceBase* game) : Game(game), Me(thing) { }
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_FishingCompetitionScript {           // the owning quest script (CLevelScriptBase<CQ_SunnyvaleMasterData>: Game at +0x40)
public:
    unsigned char _pad00[0x40];
    CGameScriptInterfaceBase* Game;           // +0x40
    class CFishCompOrganiser;
};
class CV_FishingCompetitionScript::CFishCompOrganiser : public CEntityScriptBase {   // vtable 0x012EA620
public:
    CV_FishingCompetitionScript* ParentClass; // +0x14
    CQ_SunnyvaleMasterData* PMasterData;      // +0x18''', 1)
    t = t.replace('''    CFishCompOrganiser(CScriptOwner* parent, void* masterData, const CScriptThing& me);
};

CFishCompOrganiser::CFishCompOrganiser(CScriptOwner* parent, void* masterData, const CScriptThing& me)
    : CEntityScriptBase(me, parent->m_pInterface), ParentClass(parent), seh_Hero(), seh_Organiser()
{
    PMasterData = masterData;
}''', '''    CFishCompOrganiser(CV_FishingCompetitionScript& parent, CScriptDataBase* masterData, const CScriptThing& me);
};
} // namespace NScript

NScript::CV_FishingCompetitionScript::CFishCompOrganiser::CFishCompOrganiser(CV_FishingCompetitionScript& parent, CScriptDataBase* masterData, const CScriptThing& me)
    : CEntityScriptBase(me, parent.Game), ParentClass(&parent), seh_Hero(), seh_Organiser()
{
    PMasterData = (CQ_SunnyvaleMasterData*)masterData;
}''', 1)
    assert t.count('\nNScript::CV_FishingCompetitionScript::CFishCompOrganiser::CFishCompOrganiser(') == 1, a
    _w(p, t); print('ok', a, 'src')
else: print('already', a)
# the two Alloc rows (as the landed 0x00CFA2F0 model)
ALLOC = '''// NScript::%(c)s::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new %(c)s(data, game)
// Leaf: ?Alloc@%(c)s@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x%(A)s, 40 bytes, __fastcall (data in ecx, the interface in edx).
// Identity (proven, chunk %(ch)s): new(0x%(sz)X) + ctor 0x%(ct)s (??0%(c)s@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z,
//   its own row); Ego_r.pdb / FableWin.pdb list this exact symbol.
// Callees: operator new 0x00BFEA1A, %(c)s::%(c)s 0x%(ct)s.
// Shape: retail bytes push esi ; push edi ; push 0x%(sz)x ; mov esi,edx ; mov edi,ecx ; call operator new ; add esp,4 ; test eax,eax ;
//   je N ; push esi ; push edi ; mov ecx,eax ; call ctor ; pop edi ; pop esi ; ret ; N: pop edi ; xor eax,eax ; pop esi ; ret
// Naming wave 18: landed as the free __fastcall %(old)s(void*, void*) on a %(oc)s stand-in; respelled on the PDB static
//   member (name-only: the bytes are unchanged). Base as the landed 0x00CFA2F0 Alloc row models it (CLevelScriptBase<CQ_SunnyvaleMasterData>:
//   the master data class of the five Ego_r instantiations that the game quests use).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class %(c)s : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    %(c)s(CScriptDataBase* data, CGameScriptInterfaceBase& game);   // 0x%(ct)s
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    unsigned char _members[0x%(sz)x - 0x48];                // +0x48 (the script's own members; retail sizeof 0x%(sz)X)
};
CScriptBase* __fastcall %(c)s::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new %(c)s(data, game);
}
}
typedef char ExactOwnerSize[sizeof(NScript::%(c)s) == 0x%(sz)x ? 1 : -1];
'''
put('00cf91c0', 'src', ALLOC % dict(c='CLadyGreyWifeManagerScript', A='00CF91C0', ch='c681', sz=0x94, ct='00CF88C0', old='Create_CLadyGreyWifeManagerScript_00cf91c0', oc='CLadyGreyWifeManagerScript(void*, void*)'))
put('00df1080', 'src', ALLOC % dict(c='CQ_SecretPassageScript', A='00DF1080', ch='c678', sz=0x12c, ct='00DEC7F0', old='Create_CHeroMorphDef_00df1080', oc='CHeroMorphDef'))
