// NScript::CQ_HeroSoulsArenaScript::Init() -- set up the Hero Souls arena quest: copy the script def's HeroSoulsRounds, clear the creature count, the seven state flags and the level reached, reset the guard-chat timer and add the "Arena" quest region
// Leaf: ?Init@CQ_HeroSoulsArenaScript@NScript@@UAEXXZ
// Naming wave 18: the global stand-in g_pGameScriptInterface (0x0143E8F8) is pDSTGame, a CGameScriptInterfaceBase* (ESTABLISHED_SPELLINGS; the
//   virtual slots are the base interface's) -- name-only (the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F26B90, 149 bytes, thiscall, plain `ret` (public virtual, quest vtable slot 3).
// Identity (proven): the row is referenced once, from slot 3 (Init) of the RTTI-less quest vtable 0x012D3994 (0x012D39A0),
//   which the landed CQ_HeroSoulsArenaScript ctor 0x00D7C700 and Alloc 0x00D80BA0 store (slot 1 the landed RegisterMain
//   0x00D7C740, slot 2 Main 0x00D7C890, slot 4 the landed OnPersist 0x00D7C800). Ego_r.pdb
//   ?Init@CQ_HeroSoulsArenaScript@NScript@@UAEXXZ sits at 0x00F9B570, far from the class's other members (0x00D9D3C0..
//   0x00DA2140), exactly as retail's Init sits beside the arena defs' code (0x00F26xxx). Catalog label sub_00f26b90 (gap scan).
// Callees: std::vector<CHeroSoulsRoundDef>::operator= 0x00F26C30 (landed; Ego_r ??4?$vector@VCHeroSoulsRoundDef@@...@QAEAAV01@ABV01@@Z),
//   CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0; the global interface 0x0143E8F8 slot 89 SetTimer
//   (inline CTimer::operator=(long), Ego_r ??4CTimer@@QAEAAV0@J@Z); Game slot 271 AddQuestRegion (ghidra_out/cgsi_slots_aligned.tsv).
// Data: pGlobalScriptDef 0x0143E90C; "Q_HeroSoulsArena" 0x012C3318, "Arena" 0x012C780C.
// Layout: CQ_HeroSoulsArenaScript at Ego_r - 0x14 (retail CScriptBase 0x40): CreatureCount +0x48, PlayedArenaCS +0x4c,
//   PlayerHitSummoner1 +0x4d, PlayerHitSummoner2 +0x4e, PlayerAttacksGuard +0x4f, JackTauntNearHOH +0x50, FirstWaveAttack
//   +0x51, PlayedHoHCS +0x52, ArenaLevelReached +0x54, GuardChatTimer +0x58, Rounds +0x5c (the landed OnPersist agrees).
//   CScriptDef HeroSoulsRounds retail +0x1050 = Ego_r 0x1114 - 0xc4 (the chunk 576 -0xb0 after FHTimeLimit, then 4 more for each
//   of the five Dinkumware vectors GUI_MeleeGrades / GUI_SkillGrades / GUI_WillGrades / Feats / ArenaRounds before it).
// Shape: the AddQuestRegion temporaries are built right to left ("Arena" first) and destroyed in reverse.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"                              // CCharString (agentNF60: was a local stand-in)
// agentNF60: std::vector is the shared STLport 4.6 vector (stlport/fable_containers.h; was a local namespace-std stand-in).
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#include "stlport/fable_containers.h"
class CHeroSoulsWaveDef;
// agentNF60: CHeroSoulsRoundDef is engine/CHeroSoulsRoundDef.h (through engine/CScriptDef.h; was a local model).
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/fable_script_def_global.h"          // pGlobalScriptDef 0x0143E90C (CDefPointer<const CScriptDef>; agentNF60: was a local CScriptDef model and `CScriptDef* pGlobalScriptDef`)
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
#include "engine/CTimer.h"
#include "engine/CLevelScriptBase.h"
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A (ESTABLISHED_SPELLINGS: the SetDSTGame writer)
// Retail expands CTimer::operator=(long) inline (Ego_r ??4CTimer@@QAEAAV0@J@Z): slot 89 SetTimer through 0x0143E8F8.
inline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_HeroSoulsArenaScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    long                             CreatureCount;        // +0x48
    bool                             PlayedArenaCS;        // +0x4c
    bool                             PlayerHitSummoner1;   // +0x4d
    bool                             PlayerHitSummoner2;   // +0x4e
    bool                             PlayerAttacksGuard;   // +0x4f
    bool                             JackTauntNearHOH;     // +0x50
    bool                             FirstWaveAttack;      // +0x51
    bool                             PlayedHoHCS;          // +0x52
    long                             ArenaLevelReached;    // +0x54
    CTimer                           GuardChatTimer;       // +0x58
    std::vector<CHeroSoulsRoundDef>  Rounds;               // +0x5c
    virtual void RegisterMain();                           // slot 1  0x00D7C740
    virtual void Main();                                   // slot 2  0x00D7C890
    virtual void Init();                                   // slot 3  0x00F26B90 (this row)
    virtual void OnPersist(CPersistContext& context);      // slot 4  0x00D7C800
};

void CQ_HeroSoulsArenaScript::Init()
{
    Rounds = pGlobalScriptDef->HeroSoulsRounds;
    CreatureCount = 0;
    PlayedArenaCS = false;
    PlayerHitSummoner1 = false;
    PlayerHitSummoner2 = false;
    PlayerAttacksGuard = false;
    JackTauntNearHOH = false;
    FirstWaveAttack = false;
    PlayedHoHCS = false;
    ArenaLevelReached = 0;
    GuardChatTimer = 0;
    Game->AddQuestRegion("Q_HeroSoulsArena", "Arena");
}
}   // namespace NScript