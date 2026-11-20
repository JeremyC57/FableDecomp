// CCombatManager::GetCombatantInfo(const CThingCreatureBase& creature) const -- the creature's combatant record: copy its
//   64-bit UniqueID (+0x18/+0x1c) into a local key, find it in the Combatants map at +0x10 and return the found node's
//   mapped CCombatantInfo (+0x18); the caller relies on the creature being registered.
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E9360, 41 bytes, thiscall `ret 4`.
// Identity: Ego_r.pdb names exactly two overloads, in this order: ?GetCombatantInfo@CCombatManager@@QBEABUCCombatantInfo@@
//   ABVCThingCreatureBase@@@Z (public const) then ?GetCombatantInfo@CCombatManager@@AAEAAUCCombatantInfo@@ABVCThingCreature
//   Base@@@Z (private); retail has two byte-identical bodies in the same order, 0x006E9360 then 0x006E9390. 0x006E9360 is
//   called from outside the class (CThingCreatureBase::GetShotAccuracyPercentage, CombatDialogueManager::GetBestSpeechForCreature,
//   the anonymous IsInMeleeWithCombatWheelAttackers(const CThingCreatureBase&), CMeleeCombatAbilityDef::IsAbilityValid, ...),
//   so it is the public const overload; 0x006E9390 (no direct callers) is the private one. Ego_r.pdb CCombatManager::
//   Combatants is map<unsigned __int64, CCombatantInfo> keyed by CThing::UniqueID (+0x18). The manifest's
//   CSurface::GetHeight and the catalog's CUIDMapOwner_* names are propagated / placeholders. FINDINGS chunks 407 + 408.
// Callees: the Combatants tree's node find @ 0x006F0250 (thiscall on the tree, key by reference, node pointer in eax).
// Shape: an 8-byte frame for the key copy (low word stored before the push, high word after `add ecx,10h`),
//   `lea edx,[esp] ; push edx`, then `add eax,18h`, `ret 4`.
// Leaf: ?GetCombatantInfo@CCombatManager@@QBEABUCCombatantInfo@@ABVCThingCreatureBase@@@Z
// Name fix (NF199, 2026-11-20, q4389): the invented SCombatantMap / SCombatantNode stand-ins are replaced by the STLport
//   map<unsigned __int64, CCombatantInfo> (stlport/fable_map.h); find's out-of-line _M_find<unsigned __int64> 0x006F0250 is declared
//   only (FABLE_RETAIL_STLP_TREE_MFIND_DECL, FABLE_RETAIL_STLP_TREE_CONSTFIND for the const find), so the call binds the self row's member-template spelling.
struct CCombatantInfo {                           // the mapped value (Ego_r.pdb struct CCombatantInfo; opaque here)
    long FirstWord;                                // +0x00
};

#define FABLE_RETAIL_STLP_TREE_MFIND_DECL
#define FABLE_RETAIL_STLP_TREE_CONSTFIND
#include "stlport/fable_map.h"
#include "engine/CThing.h"
class CThingCreatureBase : public CThing { };

class CCombatManager {
public:
    unsigned char  _pad00[0x10];                   // +0x00 vptr, +0x04 RegisteredCombatSequences (STLport map, 12 bytes)
    std::map<unsigned __int64, CCombatantInfo> Combatants;   // +0x10
    const CCombatantInfo& GetCombatantInfo(const CThingCreatureBase& creature) const;   // 0x006E9360
private:
    CCombatantInfo& GetCombatantInfo(const CThingCreatureBase& creature);               // 0x006E9390
};

const CCombatantInfo& CCombatManager::GetCombatantInfo(const CThingCreatureBase& creature) const
{
    unsigned __int64 key = creature.GetUniqueID();
    return Combatants.find(key)->second;
}
