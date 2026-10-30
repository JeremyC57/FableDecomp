// NScript::CQ_HeroSoulsArenaScript::Init() -- set up the Hero Souls arena quest: copy the script def's HeroSoulsRounds, clear the creature count, the seven state flags and the level reached, reset the guard-chat timer and add the "Arena" quest region
// Leaf: ?Init@CQ_HeroSoulsArenaScript@NScript@@UAEXXZ
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
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
namespace std {
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    vector& operator=(const vector& __x);               // 0x00F26C30 for CHeroSoulsRoundDef
};
}
class CHeroSoulsWaveDef;
class CHeroSoulsRoundDef : public CDefClassBase {      // 0x38 bytes (the landed operator= stride)
public:
    virtual ~CHeroSoulsRoundDef();
    virtual void Transfer(CPersistContext& context);
    virtual void Copy(const CDefClassBase* other);
    virtual unsigned long GetSizeofClass();
    long                            NumWaves;           // +0x28
    std::vector<CHeroSoulsWaveDef>  Waves;              // +0x2c
};
class CScriptDef {                                      // retail layout (Ego_r 0x1154 with Dinkumware containers)
public:
    unsigned char _before[0x1050];
    std::vector<CHeroSoulsRoundDef> HeroSoulsRounds;    // +0x1050 (Ego_r 0x1114)
};
extern CScriptDef* pGlobalScriptDef;                    // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`)
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
#include "engine/CTimer.h"
#include "engine/CLevelScriptBase.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8 (the global script interface; no PDB data symbol)
// Retail expands CTimer::operator=(long) inline (Ego_r ??4CTimer@@QAEAAV0@J@Z): slot 89 SetTimer through 0x0143E8F8.
inline CTimer& CTimer::operator=(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); return *this; }

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