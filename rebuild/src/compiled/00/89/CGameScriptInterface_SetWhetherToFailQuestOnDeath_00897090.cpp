// CGameScriptInterface::SetWhetherToFailQuestOnDeath(bool) const -- script call: sets whether the hero's death fails the current quest on the main player's hero (skipped when there is no hero, the hero is dead, or it lacks the CTCHeroStats component)
// Leaf: ?SetWhetherToFailQuestOnDeath@CGameScriptInterface@@UBEX_N@Z
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00897090, 91 bytes (96 with int3 padding), thiscall `ret 4`.
// Identity (proven): CGameScriptInterface::SetWhetherToFailQuestOnDeath; evidence: retail CGameScriptInterface vtable 0x01260F0C slot 317 holds this body, and that slot is ?SetWhetherToFailQuestOnDeath@CGameScriptInterface@@UBEX_N@Z in ghidra_out/cgsi_slots_aligned.tsv (the Ego_r.pdb CGameScriptInterface virtual order aligned to the retail table); the body tests interface 0x4 and stores a bool at +0x174, which is CTCHeroStats::FailQuestOnDeath (Ego_r.pdb +0x18C), set through FableWin.pdb ?SetWhetherToFailQuestOnDeath@CTCHeroStats@@QAEX_N@Z.
// Label: the manifest's "CGameScriptInterface::SetWhetherToFailQuestOnDeath" is propagated across family H029 (seven hero-component setters that differ only in the
//   component's interface bit, the stored field and its width).
// Callees: CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetPControlledCreature 0x00487DC0,
//   CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (CThing::GetTC<CTCHeroStats> inlined, interface 0x4).
// Model: the landed CGameScriptInterface::SetHeroAbleToGainExperience 0x00899060 (same shape, other component). Layout:
//   CGameScriptInterface +0x14 PlayerManager; CThing retail UsingComponents +0x20, Components +0x44, Dead +0x91 bit 0;
//   CTCHeroStats::FailQuestOnDeath retail +0x174 (Ego_r.pdb CTCHeroStats FailQuestOnDeath +0x18c; retail is 0x14 lower before HousePositions (retail +0x160, 12-byte vector) and 0x18 lower after it, as the landed CTCHeroStats models (HadScriptedSex +0x158, FailQuestOnDeath +0x174) pin).
// Setter: CTCHeroStats::SetWhetherToFailQuestOnDeath(bool) (FableWin.pdb ?SetWhetherToFailQuestOnDeath@CTCHeroStats@@QAEX_N@Z), inline.
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingPlayerCreature : public CThing { };
class CPlayer {
public:
    CThingPlayerCreature* GetPControlledCreature() const;   // 0x00487DC0
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
class CTCHeroStats {
public:
    unsigned char _pad00[0x174];
    bool FailQuestOnDeath;                                    // +0x174
    void SetWhetherToFailQuestOnDeath(bool value) { FailQuestOnDeath = value; }
};
class CGameScriptInterface {
public:
    virtual void SetWhetherToFailQuestOnDeath(bool value) const;
    void*           World;                               // +0x04
    void*           Component;                           // +0x08
    void*           DisplayEngine;                       // +0x0c
    void*           DefinitionManager;                   // +0x10
    CPlayerManager* PlayerManager;                       // +0x14
};

void CGameScriptInterface::SetWhetherToFailQuestOnDeath(bool value) const
{
    CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().GetPControlledCreature();
    CTCHeroStats* tc;
    if (hero && !hero->IsDead() && hero->GetTC(TCI_HERO_STATS, &tc))
        tc->SetWhetherToFailQuestOnDeath(value);
}