// CGameScriptInterface::IsToFailQuestOnDeath() const -- script call: whether the hero's death fails the current quest (CTCHeroStats), true without a live hero holding the component
// Leaf: ?IsToFailQuestOnDeath@CGameScriptInterface@@UBE_NXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe @ 0x00897030, 89 bytes, thiscall `ret`.
// Identity (proven): retail CGameScriptInterface vtable 0x01260F0C slot 316 holds this body; slot 316 is
//   ?IsToFailQuestOnDeath@CGameScriptInterface@@UBE_NXZ in ghidra_out/cgsi_slots_aligned.tsv, next to the landed
//   SetWhetherToFailQuestOnDeath 0x00897090 (slot 317). Getter: FableWin.pdb ?IsToFailQuestOnDeath@CTCHeroStats@@QBE_NXZ.
// Body: PlayerManager (+0x14)->GetMainPlayer() (0x00449970)->PeekPControlledCreature() (0x00487DD0); live hero holding
//   interface 4: PeekTC (const LowerBound 0x004365B0) and return the FailQuestOnDeath byte +0x174 (as landed 0x00897090);
//   otherwise true.
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
    unsigned char _pad00[0x174];
    bool FailQuestOnDeath;                               // +0x174
    bool IsToFailQuestOnDeath() const { return FailQuestOnDeath; }
};
class CGameScriptInterface {
public:
    void*           World;                               // +0x04
    void*           Component;                           // +0x08
    void*           DisplayEngine;                       // +0x0c
    void*           DefinitionManager;                   // +0x10
    CPlayerManager* PlayerManager;                       // +0x14
    virtual bool IsToFailQuestOnDeath() const;
};
bool CGameScriptInterface::IsToFailQuestOnDeath() const
{
    const CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().PeekPControlledCreature();
    if (hero && !hero->IsDead())
    {
        const const CTCHeroStats* stats;
        if (hero->PeekTC(TCI_HERO_STATS, &stats))
            return stats->IsToFailQuestOnDeath();
    }
    return true;
}