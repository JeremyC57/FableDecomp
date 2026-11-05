// CGameScriptInterface::GetHeroGold() const -- script call: the main player's hero's money (CTCHeroStats::GetMoney), 0 without a live hero holding the component
// Leaf: ?GetHeroGold@CGameScriptInterface@@UBEJXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe @ 0x00898FA0, 86 bytes, thiscall `ret`.
// Identity (proven): retail CGameScriptInterface vtable 0x01260F0C slot 127 holds this body; slot 127 is
//   ?GetHeroGold@CGameScriptInterface@@UBEJXZ in ghidra_out/cgsi_slots_aligned.tsv (slot 126 GiveHeroGold = 0x00898F40).
//   Getter: FableWin.pdb ?GetMoney@CTCHeroStats@@QBEJXZ, inline (+0x3c, interface 4).
// Body: PlayerManager (+0x14)->GetMainPlayer() (0x00449970)->GetPControlledCreature() (0x00487DC0); live hero holding
//   interface 4: GetTC through the non-const LowerBound 0x0040F020 and return the money word +0x3c.
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
    unsigned char _pad00[0x3c];
    long Money;                                          // +0x3c
    long GetMoney() const { return Money; }
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
long CGameScriptInterface::GetHeroGold() const
{
    CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().GetPControlledCreature();
    if (hero && !hero->IsDead())
    {
        CTCHeroStats* stats;
        if (hero->GetTC(TCI_HERO_STATS, &stats))
            return stats->GetMoney();
    }
    return 0;
}