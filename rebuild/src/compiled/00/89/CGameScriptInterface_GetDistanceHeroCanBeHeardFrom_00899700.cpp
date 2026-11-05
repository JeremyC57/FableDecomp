// CGameScriptInterface::GetDistanceHeroCanBeHeardFrom() const -- how far away the hero can be heard (CTCStealth); 0.0f when there is no live hero
// Leaf: GetDistanceHeroCanBeHeardFrom@CGameScriptInterface
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe @ 0x00899700, 101 bytes. Calls CPlayerManager::GetMainPlayer 0x00449970, CPlayer::PeekPControlledCreature 0x00487DD0,
// then CThing::PeekTC (Ego_r.pdb bool/out-pointer form; const Components lower_bound 0x004365B0).
// Shape: PeekTC's result is not tested -- without the TC bit retail reads through the uninitialised out pointer.
// Layout: CGameScriptInterface per engine/CGameScriptInterface.h (+0x14 PlayerManager); CThing retail (UsingComponents +0x20,
// Components +0x44, Dead +0x91 bit 0).
// CTCStealth (Ego_r.pdb, interface 0x36): DistanceCanBeHeardFrom +0x14 (no STL member precedes it).
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
class CTCStealth {
public:
    unsigned char _pad00[0x14];
    float         DistanceCanBeHeardFrom;                // +0x14
    float GetDistanceCanBeHeardFrom() const { return DistanceCanBeHeardFrom; }
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

float CGameScriptInterface::GetDistanceHeroCanBeHeardFrom() const
{
    const CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().PeekPControlledCreature();
    if (hero && !hero->IsDead())
    {
        const const CTCStealth* tc;
        hero->PeekTC(TCI_STEALTH, &tc);
        return tc->GetDistanceCanBeHeardFrom();
    }
    return 0.0f;
}