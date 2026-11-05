// CGameScriptInterface::GetHeroRoughExperienceLevel() const -- the hero's rough experience level; 0 when there is no live hero
// Leaf: GetHeroRoughExperienceLevel@CGameScriptInterface
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe @ 0x00899770, 101 bytes. Calls CPlayerManager::GetMainPlayer 0x00449970, CPlayer::PeekPControlledCreature 0x00487DD0,
// then CThing::PeekTC (Ego_r.pdb bool/out-pointer form; const Components lower_bound 0x004365B0); CTCHeroExperience::GetRoughExperienceLevel 0x006D64B0.
// Shape: the out pointer is null-tested, not PeekTC's result (retail reads it uninitialised when the TC bit is clear).
// Layout: CGameScriptInterface per engine/CGameScriptInterface.h (+0x14 PlayerManager); CThing retail (UsingComponents +0x20,
// Components +0x44, Dead +0x91 bit 0).
// CTCHeroExperience (Ego_r.pdb, interface 0x68): ExperiencePointsAvailableToSpend +0x14 (no STL member precedes it).
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingPlayerCreature : public CThing { };
class CPlayer {
public:
    CThingPlayerCreature* GetPControlledCreature() const;       // 0x00487DC0
    const CThingPlayerCreature* PeekPControlledCreature() const;      // 0x00487DD0
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
enum EHeroExperienceStatCategory { HERO_EXPERIENCE_STAT_STRENGTH = 0, HERO_EXPERIENCE_STAT_SKILL = 1, HERO_EXPERIENCE_STAT_WILL = 2,
                                   HERO_EXPERIENCE_STAT_FORCE_DWORD = 0x7fffffff };
enum EHeroTrainableStatType { HERO_TRAINABLE_STAT_FORCE_DWORD = 0x7fffffff };
class CTCHeroExperience {
public:
    unsigned char _pad00[0x14];
    long          ExperiencePointsAvailableToSpend;      // +0x14
    long GetExperiencePointsAvailableToSpend() const { return ExperiencePointsAvailableToSpend; }
    long GetRoughExperienceLevel() const;                                         // 0x006D64B0
    long GetTotalStatLevel(EHeroExperienceStatCategory category) const;          // 0x006D6660
    long GetTrainableStatLevel(EHeroTrainableStatType stat) const;               // 0x006D6520
    long GetTrainableStatMaxLevel(EHeroTrainableStatType stat) const;            // 0x006D6770
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

long CGameScriptInterface::GetHeroRoughExperienceLevel() const
{
    const CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().PeekPControlledCreature();
    if (hero && !hero->IsDead())
    {
        const const CTCHeroExperience* tc;
        hero->PeekTC(TCI_HERO_EXPERIENCE, &tc);
        if (tc)
            return tc->GetRoughExperienceLevel();
    }
    return 0;
}