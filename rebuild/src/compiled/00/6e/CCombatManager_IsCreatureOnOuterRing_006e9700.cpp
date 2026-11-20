// CCombatManager::IsCreatureOnOuterRing(const CThingCreatureBase& creature) const -- false for a creature that is not combat-eligible; true when it is not using its victim's combat wheel or holds no ring, else whether its ring is the wheel's outer ring for the victim's combat type
// Leaf: ?IsCreatureOnOuterRing@CCombatManager@@QBE_NABVCThingCreatureBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006E9700, 145 bytes (160 with int3 padding), thiscall, `ret 4`.
// Identity: Ego_r.pdb name set `?IsCreatureOnOuterRing@CCombatManager@@QBE_NABVCThingCreatureBase@@@Z` (the one
//   CCombatManager bool query that ends in CCombatWheel::IsCombatRingOuterRing). Shape and layout as the landed
//   CCombatManager::SetAttackerRingAsRotating 0x006E9D90. The catalog's "CCombatWheel_ValidateSequencePosition" is
//   descriptive.
// Callees: the creature combat-eligibility test @ 0x00662410, the combatant map find(const unsigned __int64&) @ 0x006F0250
//   (twice), CThingCreatureBase::GetCombatTypeDef() @ 0x00666D10, CCombatWheel::IsCombatRingOuterRing(const
//   CCombatTypeDef&, long) const @ 0x006E84A0 (landed).
// Layout: CCombatManager Combatants +0x10 (STLport map, node value +0x18); CCombatantInfo AttackerInfo +4 (Attacked +0,
//   CombatWheelRingIndex +8, UsingCombatWheel +0x1C), CombatWheel +0x40.
// Shape: one `if (eligible) { ...; return true; } return false;` block (all three callee-saved registers pushed up
//   front; an early `return false` shrink-wraps ebx and duplicates epilogues); the attacker's info is a reference
//   (`add esi,18h` at once) but the victim's entry is kept as the find's node (`lea ecx,[ebx+58h]` at the use), looked
//   up before the ring index is tested; `index < 0 || outer` returns true through the shared block. The outer-ring
//   call pushes the index, then the GetCombatTypeDef() result. The find has a same-TU noinline body (one shared key
//   slot, as 0x006E93C0).
// Name fix (NF207, 2026-11-20, q4389, family FP-COMBATMAP): the invented CRetailRbMap_UID_CombatantInfo stand-in (and its same-TU
//   noinline find body) is replaced in place by the STLport map<unsigned __int64, CCombatantInfo> (stlport/fable_map.h); its
//   _M_find<unsigned __int64> 0x006F0250 is visible but out of line (FABLE_RETAIL_STLP_TREE_MFIND_NOINLINE: the pick-any copy is
//   byte- and relocation-identical to the 0x006F0250 row), CONSTFIND gives the const find. The victim's entry is now read through
//   the inline GetCombatantInfo reference (as the landed 0x006E9D90 / 0x006E94D0; c2 still keeps the node and adds 58h at the
//   use): a named const_iterator for it let VC7.1 sink the second _M_find call below the ring-index test (DIFFER 146v145), and the
//   declaration-only _M_find gives the second key its own slot (frame 10h v 8, NF195).
#define FABLE_RETAIL_STLP_TREE_MFIND_NOINLINE
#define FABLE_RETAIL_STLP_TREE_CONSTFIND
#include "stlport/fable_map.h"
class CCombatTypeDef;

class CThingCreatureBase {
public:
    unsigned char    _pad[0x18];
    unsigned __int64 UniqueID;                              // +0x18
    unsigned __int64 GetUID() const { return UniqueID; }
    const CCombatTypeDef* GetCombatTypeDef() const;         // 0x00666D10
public:
    bool IsInMeleeAsAttacker() const;   // 0x00662410
};

template <class T> struct CRetailVector {                   // retail std::vector: {first, last, end_of_storage}
    T* first;
    T* last;
    T* end_of_storage;
};

struct CCombatRing;
struct CCombatWheel {                                       // CDefendingCombatantInfo::CCombatWheel
    CRetailVector<CCombatRing> CombatRings;                 // +0x00
    bool IsCombatRingOuterRing(const CCombatTypeDef& def, long index) const;   // 0x006E84A0
};

class CAttackingCombatantInfo { public:
    const CThingCreatureBase* Attacked;                     // +0x00
    long AttackerNumber;                                    // +0x04
    long CombatWheelRingIndex;                              // +0x08
    bool PreferredAttackDollySet;                           // +0x0C
    long PreferredAttackDollyTimer;                         // +0x10
    float PreferredAttackDollyVector[2];                    // +0x14 C2DVector
    bool UsingCombatWheel;                                  // +0x1C
    unsigned char _rest[0x30 - 0x1d];
};

struct CCombatantInfo {                                     // retail layout (Ego_r.pdb names; 12-byte vectors/maps)
    const CThingCreatureBase* Parent;                       // +0x00
    CAttackingCombatantInfo AttackerInfo;                   // +0x04
    unsigned char Attackers[0x0c];                          // +0x34
    CCombatWheel CombatWheel;                               // +0x40
};


class CCombatManager {
public:
    virtual void v00();
    unsigned char _sequences[0xc];                          // +0x04
    std::map<unsigned __int64, CCombatantInfo> Combatants;              // +0x10
    const CCombatantInfo& GetCombatantInfo(const CThingCreatureBase& creature) const
    {
        return Combatants.find(creature.GetUID())->second;
    }
    bool IsCreatureOnOuterRing(const CThingCreatureBase& creature) const;
};

bool CCombatManager::IsCreatureOnOuterRing(const CThingCreatureBase& creature) const
{
    if (creature.IsInMeleeAsAttacker()) {
        const CCombatantInfo& info = GetCombatantInfo(creature);
        if (!info.AttackerInfo.UsingCombatWheel)
            return true;
        const CThingCreatureBase* attacked = info.AttackerInfo.Attacked;
        const CCombatantInfo& attackedInfo = GetCombatantInfo(*attacked);
        long ringIndex = info.AttackerInfo.CombatWheelRingIndex;
        if (ringIndex < 0
            || attackedInfo.CombatWheel.IsCombatRingOuterRing(*attacked->GetCombatTypeDef(), ringIndex))
            return true;
    }
    return false;
}