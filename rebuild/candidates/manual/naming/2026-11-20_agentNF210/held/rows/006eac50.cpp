// 006eac50 CCombatManager::GetCombatDebugInfo
// Flags: /O2 /Oy /W3
// Leaf: ?GetCombatDebugInfo@CCombatManager@@QBE?AVCCombatDebugInfo@@ABVCThingCreatureBase@@@Z
// CCombatManager::GetCombatDebugInfo(const CThingCreatureBase&) const -- a debug snapshot of a creature's melee state: whether it is an
// attacker (then its defender zone, the current sequence name cut after 15 characters when longer, its attacker number) and whether it is
// attacked; returned by value.
// Retail: Fable.exe (TLC, VC7.1) @ 0x006EAC50. Callees: CThingCreatureBase::IsInMeleeAsAttacker 0x00662410 / IsInMeleeAsAttacked 0x00662430,
//   the combatant map's _M_find<unsigned __int64> 0x006F0250 (through the inline GetCombatantInfo), GetDefenderZone 0x006E99B0, the
//   sequence's virtual GetName (slot 2), CCharString members, the CCombatDebugInfo copy constructor (0x006F3000) and destructor (0x006F2FD0).
// Name fix (NF210, 2026-11-20, q4389, family FP-COMBATMAP): self leaf ?GetCombatDebugInfo@CCombatManager@CombatDebug@@QBE?AUCCombatDebugInfo@2@..
//   (namespaced stand-in) -> the Ego_r.pdb (0x00715140) + FableWin.pdb (0x0241C2D0) spelling above. The row is ported onto the shared headers:
//   engine/CCombatManager.h (FABLE_RETAIL_CCOMBATMANAGER_GETCOMBATDEBUGINFO, GETCOMBATANTINFO_INLINE), engine/CThingCreatureBase.h
//   (FABLE_RETAIL_CTHINGCREATUREBASE_ISINMELEEASATTACKED), engine/CCombatantInfo.h, fable_string.h, engine/C3DVector.h and
//   stlport/fable_map.h (MFIND_NOINLINE + CONSTFIND: the HEAD find had a same-TU noinline body). The macro-renamed include of
//   engine/CThingCreatureBase.h and the namespaced CThing / CCharString / C3DVector / vector / CCountedPointer / CAttackingCombatantInfo /
//   CCombatantInfo / CombatantMap / CCombatManager models are gone. Kept in place (no shared header): CCombatDebugInfo (HEAD layout; Ego_r.pdb
//   members; the landed owner of its destructor, 0x006F2FD0, models the same global class) and CCombatSequenceBase (engine/_quarantine
//   only; landed 0x006E97F0 models it globally), both moved out of the CombatDebug namespace (same class tokens as HEAD).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ISINMELEEASATTACKED
#define FABLE_RETAIL_CCOMBATMANAGER
#define FABLE_RETAIL_CCOMBATMANAGER_GETCOMBATDEBUGINFO
#define FABLE_RETAIL_CCOMBATMANAGER_GETCOMBATANTINFO_INLINE
#define FABLE_RETAIL_STLP_TREE_MFIND_NOINLINE
#define FABLE_RETAIL_STLP_TREE_CONSTFIND
#include "stlport/fable_map.h"
#include <string.h>
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/ECombatZone.h"
class CCombatSequenceBase { public:                // quarantined header: header-less for the local-model rule
    virtual ~CCombatSequenceBase();
    virtual void Delete();
    virtual CCharString GetName() const;           // slot 2 (+8)
};
class CCombatDebugInfo { public:                   // no shared header; the HEAD layout
    long Zone;                                     // +0x00
    bool Attacker;                                 // +0x04
    long AttackerNumber;                           // +0x08
    bool Defender;                                 // +0x0c
    bool MovingByAction;                           // +0x0d
    CCharString CurrentSequenceName;               // +0x10
    std::vector<C3DVector> PlacementVectors;       // +0x14
    std::vector<C3DVector> AttackerPos;            // +0x20
    CCombatDebugInfo() { memset(this, 0, sizeof(*this)); }
};
#include "engine/CThingCreatureBase.h"
#include "engine/CCombatManager.h"

CCombatDebugInfo CCombatManager::GetCombatDebugInfo(const CThingCreatureBase& creature) const
{
    CCombatDebugInfo info;
    info.Attacker = creature.IsInMeleeAsAttacker();
    if (info.Attacker) {
        const CCombatantInfo& combatant = GetCombatantInfo(creature);
        info.Zone = GetDefenderZone(*combatant.AttackerInfo.Attacked, creature, 0);
        info.CurrentSequenceName = combatant.AttackerInfo.CurrentSequence >= 0
            ? combatant.AttackerInfo.CombatSequences[combatant.AttackerInfo.CurrentSequence]->GetName() : CCharString("");
        if (info.CurrentSequenceName.GetLength() > 15) {
            CCharString shortened;
            shortened = info.CurrentSequenceName.GetAsCharArray() + 15;
            info.CurrentSequenceName = shortened;
        }
        info.AttackerNumber = combatant.AttackerInfo.AttackerNumber;
    }
    info.Defender = creature.IsInMeleeAsAttacked();
    return info;
}
