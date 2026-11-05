// CGameScriptInterface::GetNumberOfTimesHeroHasHadSex() const -- script call: how many times the main player's hero has had sex (CTCHeroStats), 0 without a live hero holding the component
// Leaf: ?GetNumberOfTimesHeroHasHadSex@CGameScriptInterface@@UBEJXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe @ 0x00897C90, 89 bytes, thiscall `ret`.
// Identity (proven): retail CGameScriptInterface vtable 0x01260F0C slot 218 holds this body; slot 218 is
//   ?GetNumberOfTimesHeroHasHadSex@CGameScriptInterface@@UBEJXZ in ghidra_out/cgsi_slots_aligned.tsv, next to the landed
//   SetNumberOfTimesHeroHasHadSex 0x00897CF0 (slot 219). Getter: FableWin.pdb ?GetNumberOfTimesHadSex@CTCHeroStats@@QBEJXZ, inline.
// Body: PlayerManager (+0x14)->GetMainPlayer() (0x00449970)->GetPControlledCreature() (0x00487DC0); if the hero exists, is
//   not dead (+0x91 bit 0) and holds interface 4 (UsingComponents +0x20 bit 4): PeekTC through the const LowerBound
//   0x004365B0 and return CTCHeroStats::NumSex (+0x154, as landed 0x00897CF0).
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingPlayerCreature : public CThing { };
class CPlayer {
public:
    CThingPlayerCreature* GetPControlledCreature() const;    // 0x00487DC0
    const CThingPlayerCreature* PeekPControlledCreature() const;   // 0x00487DD0
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
class CTCHeroStats {                                    // interface 4
public:
    unsigned char _pad00[0x154];
    long NumSex;                                         // +0x154
    long GetNumberOfTimesHadSex() const { return NumSex; }
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
long CGameScriptInterface::GetNumberOfTimesHeroHasHadSex() const
{
    CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().GetPControlledCreature();
    if (hero && !hero->IsDead())
    {
        const const CTCHeroStats* stats;
        if (hero->PeekTC(TCI_HERO_STATS, &stats))
            return stats->GetNumberOfTimesHadSex();
    }
    return 0;
}