# h16_qctor.py: naming wave 16, family qctor -- the c671/c673 quest-script constructors landed with (void* / CScriptDataBase*,
# CGameScriptInterface* / &) parameters are respelled on the proven PDB constructor
# ??0<C>@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z (the argument-storing intermediate base takes the
# same pair and keeps Ego_r CLevelScriptBase<T>'s CGameScriptInterfaceBase* Game); CV_SwordInTheStoneScript moves into
# namespace NScript; the user destructors retail does not declare (TalentlessBard / TourGuide / FisticuffsClub: implicit in
# retail, chunk c673) are dropped; the two Create_CClass_* factories are respelled as the PDB ?Alloc@<C>@NScript@@SI...
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit, path, _w
P = 'CScriptDataBase* data, CGameScriptInterfaceBase& game'
def leaf(c): return '??0%s@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z' % c
NOTE = ('// Naming wave 16 (%s): the PDB parameter pair (CScriptDataBase*, CGameScriptInterfaceBase&) replaces the landed %s;\n'
        '//   name-only (both are one pushed pointer: the bytes are unchanged).\n')
# --- 0x00ED4AA0 CV_SwordInTheStoneScript
a = '00ed4aa0'
edit(a, 'src', '// CV_SwordInTheStoneScript::CV_SwordInTheStoneScript(void* data, CGameScriptInterface* game) --',
     '// NScript::CV_SwordInTheStoneScript::CV_SwordInTheStoneScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) --')
edit(a, 'src', '// Leaf: ??0CV_SwordInTheStoneScript\n', '// Leaf: %s\n' % leaf('CV_SwordInTheStoneScript') +
     NOTE % ('c671', '(void*, CGameScriptInterface*) and global-namespace class') )
edit(a, 'src', '    CGameScriptInterface* Game;                    // +0x40 (second argument)\n    void*                 Data;                    // +0x44 (first argument)\n    CQuestScriptObject(void* data, CGameScriptInterface* game) : CScriptBase(), Game(game), Data(data) { }',
     '    CGameScriptInterfaceBase* Game;                // +0x40 (second argument; Ego_r CLevelScriptBase<T>::Game)\n    CScriptDataBase*          Data;                // +0x44 (first argument)\n    CQuestScriptObject(%s) : CScriptBase(), Game(&game), Data(data) { }' % P)
edit(a, 'src', 'struct CV_SwordInTheStoneScript : CQuestScriptObject {   // vtable 0x012F301C\n',
     'namespace NScript {\nclass CV_SwordInTheStoneScript : public CQuestScriptObject {   // vtable 0x012F301C\npublic:\n')
edit(a, 'src', '    CV_SwordInTheStoneScript(void* data, CGameScriptInterface* game);\n};\nCV_SwordInTheStoneScript::CV_SwordInTheStoneScript(void* data, CGameScriptInterface* game)\n',
     '    CV_SwordInTheStoneScript(%s);\n};\n} // namespace NScript\nNScript::CV_SwordInTheStoneScript::CV_SwordInTheStoneScript(%s)\n' % (P, P))
# --- 0x00ED76E0 CV_TalentlessBardScript
a = '00ed76e0'
edit(a, 'src', '// CV_TalentlessBardScript::CV_TalentlessBardScript(CScriptDataBase*,CGameScriptInterface*) --',
     '// NScript::CV_TalentlessBardScript::CV_TalentlessBardScript(CScriptDataBase*, CGameScriptInterfaceBase&) --')
edit(a, 'src', '// Retail0x00ED76E0,164 bytes.', '// Leaf: %s\n' % leaf('CV_TalentlessBardScript') +
     NOTE % ('c673', '(CScriptDataBase*, CGameScriptInterface*); the user ~CV_TalentlessBardScript the landed model declared is\n'
             '//   dropped (retail\'s 0x00EDAD10 is the implicit destructor, chunk c673)') + '// Retail0x00ED76E0,164 bytes.')
edit(a, 'src', '#include "engine/CGameScriptInterface.h"\nclass CScriptDataBase;\n', '#include "engine/CGameScriptInterface.h"\nclass CScriptDataBase;\nclass CGameScriptInterfaceBase;\n')
edit(a, 'src', '    CGameScriptInterface* Game;\n    CScriptDataBase* Data;\n    CQuestScriptObjectNative(CScriptDataBase* data,CGameScriptInterface* game)\n        : CScriptBaseNative(),Game(game),Data(data) {}',
     '    CGameScriptInterfaceBase* Game;                // Ego_r CLevelScriptBase<T>::Game\n    CScriptDataBase* Data;\n    CQuestScriptObjectNative(%s)\n        : CScriptBaseNative(),Game(&game),Data(data) {}' % P)
edit(a, 'src', 'public:\n    virtual ~CV_TalentlessBardScript();\n', 'public:\n')
edit(a, 'src', '    CV_TalentlessBardScript(CScriptDataBase*,CGameScriptInterface*);\n', '    CV_TalentlessBardScript(%s);\n' % P)
edit(a, 'src', 'NScript::CV_TalentlessBardScript::CV_TalentlessBardScript(CScriptDataBase* data,CGameScriptInterface* game)\n',
     'NScript::CV_TalentlessBardScript::CV_TalentlessBardScript(%s)\n' % P)
# --- 0x00EE41F0 CV_TourGuideScript
a = '00ee41f0'
edit(a, 'src', '// NScript::CV_TourGuideScript::CV_TourGuideScript(CScriptDataBase* data, CGameScriptInterface& game) --',
     '// NScript::CV_TourGuideScript::CV_TourGuideScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) --')
edit(a, 'src', '// Leaf: ??0CV_TourGuideScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterface@@@Z\n', '// Leaf: %s\n' % leaf('CV_TourGuideScript') +
     NOTE % ('c673', 'CGameScriptInterface& parameter; the user ~CV_TourGuideScript the landed model declared is dropped\n'
             '//   (retail\'s 0x00EE6C50 is the implicit destructor, chunk c673)'))
edit(a, 'src', 'class CGameScriptInterface;\nclass CScriptDataBase;\n', 'class CGameScriptInterfaceBase;\nclass CScriptDataBase;\n')
edit(a, 'src', '    CGameScriptInterface* Game;                         // +0x40\n', '    CGameScriptInterfaceBase* Game;                     // +0x40\n')
edit(a, 'src', '    CLevelScriptBase(CScriptDataBase* data, CGameScriptInterface& game) : CScriptBase(), Game(&game),',
     '    CLevelScriptBase(%s) : CScriptBase(), Game(&game),' % P)
edit(a, 'src', '    virtual ~CV_TourGuideScript();\n    CV_TourGuideScript(CScriptDataBase* data, CGameScriptInterface& game);\n',
     '    CV_TourGuideScript(%s);\n' % P)
edit(a, 'src', 'CV_TourGuideScript::CV_TourGuideScript(CScriptDataBase* data, CGameScriptInterface& game)\n',
     'CV_TourGuideScript::CV_TourGuideScript(%s)\n' % P)
# --- 0x00CFA660 CQ_ArenaScript
a = '00cfa660'
edit(a, 'src', '// CQ_ArenaScript::CQ_ArenaScript(void* owner, CGameScriptInterface* game) --',
     '// NScript::CQ_ArenaScript::CQ_ArenaScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) --')
edit(a, 'src', '// Leaf: ??0CQ_ArenaScript\n', '// Leaf: %s\n' % leaf('CQ_ArenaScript') + NOTE % ('c673', '(void* owner, CGameScriptInterface*)'))
edit(a, 'src', '    CGameScriptInterface* m_pGame;                   // +0x40\n    void* m_pOwner;                                  // +0x44\n    CScriptObject(void* owner, CGameScriptInterface* game) : CScriptBase(), m_pGame(game), m_pOwner(owner) { }',
     '    CGameScriptInterfaceBase* m_pGame;               // +0x40 (Ego_r CLevelScriptBase<T>::Game)\n    CScriptDataBase* m_pOwner;                       // +0x44\n    CScriptObject(%s) : CScriptBase(), m_pGame(&game), m_pOwner(data) { }' % P)
edit(a, 'src', '    CQ_ArenaScript(void* owner, CGameScriptInterface* game);\n', '    CQ_ArenaScript(%s);\n' % P)
edit(a, 'src', 'NScript::CQ_ArenaScript::CQ_ArenaScript(void* owner, CGameScriptInterface* game)\n    : CScriptObject(owner, game)',
     'NScript::CQ_ArenaScript::CQ_ArenaScript(%s)\n    : CScriptObject(data, game)' % P)
# --- 0x00E81F60 CV_FisticuffsClubScript
a = '00e81f60'
edit(a, 'src', '// CV_FisticuffsClubScript::CV_FisticuffsClubScript(void* data, CGameScriptInterface* game) --',
     '// NScript::CV_FisticuffsClubScript::CV_FisticuffsClubScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) --')
edit(a, 'src', '// Leaf: ??0CV_FisticuffsClubScript\n', '// Leaf: %s\n' % leaf('CV_FisticuffsClubScript') +
     NOTE % ('c673', '(void*, CGameScriptInterface*); the user ~CV_FisticuffsClubScript the landed model declared is dropped\n'
             '//   (retail\'s 0x00E8B750 is the implicit destructor, chunk c673)'))
edit(a, 'src', '    CGameScriptInterface* Game;                    // +0x40 (second argument)\n',
     '    CGameScriptInterfaceBase* Game;                // +0x40 (second argument; Ego_r CLevelScriptBase<T>::Game)\n')
edit(a, 'src', '    CQuestScriptObject(void* data, CGameScriptInterface* game) : CScriptBase(), Game(game), Data(data) { }',
     '    CQuestScriptObject(%s) : CScriptBase(), Game(&game), Data(data) { }' % P)
edit(a, 'src', '    virtual ~CV_FisticuffsClubScript();\n    CV_FisticuffsClubScript(void* data, CGameScriptInterface* game);\n',
     '    CV_FisticuffsClubScript(%s);\n' % P)
edit(a, 'src', 'NScript::CV_FisticuffsClubScript::CV_FisticuffsClubScript(void* data, CGameScriptInterface* game)\n',
     'NScript::CV_FisticuffsClubScript::CV_FisticuffsClubScript(%s)\n' % P)
# --- 0x00EDEBA0 CV_TravellingHeroesScript
a = '00edeba0'
edit(a, 'src', '// CV_TravellingHeroesScript::CV_TravellingHeroesScript(CScriptDataBase*, CGameScriptInterface&) --',
     '// NScript::CV_TravellingHeroesScript::CV_TravellingHeroesScript(CScriptDataBase*, CGameScriptInterfaceBase&) --')
edit(a, 'src', '// Leaf: ??0CV_TravellingHeroesScript\n', '// Leaf: %s\n' % leaf('CV_TravellingHeroesScript') + NOTE % ('c673', '(CScriptDataBase*, CGameScriptInterface&)'))
edit(a, 'src', '    CGameScriptInterface* GSI;                           // +0x40\n', '    CGameScriptInterfaceBase* GSI;                       // +0x40 (Ego_r CLevelScriptBase<T>::Game)\n')
edit(a, 'src', '    CQuestScriptBase(CScriptDataBase* data, CGameScriptInterface* gsi) : CScriptBase(), GSI(gsi), Data(data) {}',
     '    CQuestScriptBase(CScriptDataBase* data, CGameScriptInterfaceBase& gsi) : CScriptBase(), GSI(&gsi), Data(data) {}')
edit(a, 'src', '    CV_TravellingHeroesScript(CScriptDataBase* data, CGameScriptInterface& gsi);\n', '    CV_TravellingHeroesScript(CScriptDataBase* data, CGameScriptInterfaceBase& gsi);\n')
edit(a, 'src', 'NScript::CV_TravellingHeroesScript::CV_TravellingHeroesScript(CScriptDataBase* data, CGameScriptInterface& gsi)\n    : CQuestScriptBase(data, &gsi)',
     'NScript::CV_TravellingHeroesScript::CV_TravellingHeroesScript(CScriptDataBase* data, CGameScriptInterfaceBase& gsi)\n    : CQuestScriptBase(data, gsi)')
# --- the two factories: ?Alloc@<C>@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
ALLOC = '''// NScript::%(c)s::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- the script-info factory: new %(c)s(data, game)
// Leaf: ?Alloc@%(c)s@NScript@@SIPAVCScriptBase@@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x%(A)s, 40 bytes, __fastcall (data in ecx, the interface in edx).
// Identity (proven, chunk c673): the factory the script-info registration names for %(c)s; `push %(sz)sh` = the retail
//   sizeof, then the constructor 0x%(C)s (??0%(c)s@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z, its own row)
//   on the new block; the exact PDB signature (as the landed Alloc rows, e.g. 0x00D61BB0 CQ_GuildTrainingWillScript::Alloc).
// Callees: operator new 0x00BFEA1A, %(c)s::%(c)s 0x%(C)s.
// Shape: retail bytes push esi ; push edi ; push %(sz)sh ; mov esi,edx ; mov edi,ecx ; call operator new ; test ; je ; push esi ;
//   push edi ; mov ecx,eax ; call ctor ; ... -- the constructor out of line, the null-tested `new`.
// Naming wave 16: landed as the free __fastcall Create_CClass_%(c8)s_%(a)s(void*, void*) on a CClass_%(c8)s stand-in;
//   respelled on the PDB static member (name-only: the bytes are unchanged).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class %(c)s : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    %(c)s(CScriptDataBase* data, CGameScriptInterfaceBase& game);   // 0x%(C)s
    static CScriptBase* __fastcall Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    unsigned char _members[0x%(sz)s - 0x48];                 // +0x48 (the script's own members; retail sizeof 0x%(sz)s)
};
CScriptBase* __fastcall %(c)s::Alloc(CScriptDataBase* data, CGameScriptInterfaceBase& game)
{
    return new %(c)s(data, game);
}
}
typedef char ExactOwnerSize[sizeof(NScript::%(c)s) == 0x%(sz)s ? 1 : -1];
'''
for a, c, ctor, sz in (('00ee6c00', 'CV_TourGuideScript', '00ee41f0', '178'), ('00e8b700', 'CV_FisticuffsClubScript', '00e81f60', 'c8')):
    p, rel = path(a, 'src'); t = ALLOC % dict(a=a, A=a.upper(), c=c, C=ctor.upper(), c8=ctor, sz=sz)
    if open(p, encoding='utf-8', newline='').read() == t: print('already', a)
    else: _w(p, t); print('ok', a, 'src')
