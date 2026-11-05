// CGameScriptInterface::UpdateScore_FishingCompetition(float) const -- script call: records the hero's fishing-competition score as the biggest fish on the main player's hero (skipped when there is no hero, the hero is dead, or it lacks the CTCHeroStats component)
// Leaf: ?UpdateScore_FishingCompetition@CGameScriptInterface@@UBEXM@Z
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008A1240, 91 bytes (96 with int3 padding), thiscall `ret 4`.
// Identity (proven): CGameScriptInterface::UpdateScore_FishingCompetition; evidence: retail CGameScriptInterface vtable 0x01260F0C slot 729 holds this body, and that slot is ?UpdateScore_FishingCompetition@CGameScriptInterface@@UBEXM@Z in ghidra_out/cgsi_slots_aligned.tsv (the Ego_r.pdb CGameScriptInterface virtual order aligned to the retail table); the body tests interface 0x4 and stores a float at +0x15C, which is CTCHeroStats::BiggestFish (Ego_r.pdb +0x170), set through FableWin.pdb ?SetBiggestFish@CTCHeroStats@@QAEXM@Z.
// Label: the manifest's "CGameScriptInterface::UpdateScore_FishingCompetition" is propagated across family H029 (seven hero-component setters that differ only in the
//   component's interface bit, the stored field and its width).
// Callees: CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetPControlledCreature 0x00487DC0,
//   CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (CThing::GetTC<CTCHeroStats> inlined, interface 0x4).
// Model: the landed CGameScriptInterface::SetHeroAbleToGainExperience 0x00899060 (same shape, other component). Layout:
//   CGameScriptInterface +0x14 PlayerManager; CThing retail UsingComponents +0x20, Components +0x44, Dead +0x91 bit 0;
//   CTCHeroStats::BiggestFish retail +0x15c (Ego_r.pdb CTCHeroStats BiggestFish +0x170; retail is 0x14 lower before HousePositions (retail +0x160, 12-byte vector) and 0x18 lower after it, as the landed CTCHeroStats models (HadScriptedSex +0x158, FailQuestOnDeath +0x174) pin).
// Setter: CTCHeroStats::SetBiggestFish(float) (FableWin.pdb ?SetBiggestFish@CTCHeroStats@@QAEXM@Z), inline.
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
    unsigned char _pad00[0x15c];
    float BiggestFish;                                    // +0x15c
    void SetBiggestFish(float value) { BiggestFish = value; }
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::UpdateScore_FishingCompetition(float value) const
{
    CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().GetPControlledCreature();
    CTCHeroStats* tc;
    if (hero && !hero->IsDead() && hero->GetTC(TCI_HERO_STATS, &tc))
        tc->SetBiggestFish(value);
}