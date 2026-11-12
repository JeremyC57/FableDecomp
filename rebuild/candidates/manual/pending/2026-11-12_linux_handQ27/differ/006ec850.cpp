// CDefendingCombatantInfo::CCombatWheel::Init(CCombatManager&, CWorldMap& world, const map<CThingCreatureBase*, CAttackingCombatantInfo*>& attackers, const CThingCreatureBase& defender) -- split the defender's attackers into inner-ring and outer-ring-preferring sets (melee outer attackers queued first), move outer attackers inward when the inner set is empty (or every outer one when there is no inner ring), reset every ring, fill the inner rings then the outer rings (adding rings as needed), shave off the unused rings and mark every unplaced attacker's ring index -1
// Grade: DIFFER(1606v1606) frame 0x3c v retail 0x30 only: retail shares one stack slot between the PeekTC interface-type key and all three map insert_unique pair<iterator,bool> results; VC7.1 gives our pairs their own slots (braces, named results, out-of-line insert, default PeekTC tried: unchanged)
// Leaf: ?Init@CCombatWheel@CDefendingCombatantInfo@@QAEXAAVCCombatManager@@AAVCWorldMap@@ABV?$map@PAVCThingCreatureBase@@PAVCAttackingCombatantInfo@@U?$less@PAVCThingCreatureBase@@@std@@V?$allocator@U?$pair@QAVCThingCreatureBase@@PAVCAttackingCombatantInfo@@@std@@@4@@std@@ABVCThingCreatureBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006EC850, 1606 bytes, thiscall, `ret 10h` (the CCombatManager& is not read).
// Identity: Ego_r.pdb / FableWin.pdb ?Init@CCombatWheel@CDefendingCombatantInfo@@QAEXAAVCCombatManager@@AAVCWorldMap@@ABV?$map@..@std@@
//   ABVCThingCreatureBase@@@Z -- the only CCombatWheel member with four arguments (ret 10h) taking the attacker map and the
//   defender; its body calls AddRing / CCombatRing::Reset / AssignAttackersToRing (the manifest's "PopulateCombatRings" is a
//   made-up label).
// Callees: malloc 0x00BFEA0E / free 0x00BFEA14 (STLport allocator), CThingCreatureBase::GetCombatTypeDef 0x00666D10,
//   CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound (const) 0x004365B0, CTCCarrying::GetThingInPrimarySlot 0x006A1440,
//   CTCWeapon::IsMeleeWeapon 0x005DAA00, map<CThingCreatureBase*,CAttackingCombatantInfo*> insert_unique 0x006F2A40 /
//   _M_erase 0x006EFCE0, _Rb_global<bool>::_Rebalance_for_erase 0x0042A34E, CCombatRing::Reset 0x006E8B60 /
//   AssignAttackersToRing 0x006EBF70, CCombatWheel::AddRing 0x006EC7D0, vector<CCombatRing> erase's copy 0x006F4470,
//   ~CIntelligentPointer 0x00A01C10.
// Layout: engine/CDefendingCombatantInfo.h (CombatRings +0), CCombatTypeDef (DefenderCombatRingOccupationNumbers +0x3c,
//   DefenderWheelOuterRingStartIndex +0x48, PreferOuterRing +0x68), CAttackingCombatantInfo::CombatWheelRingIndex +8,
//   CTCCombat UnsheathingWeaponType +0x6c (GetUnsheathingWeaponType).
#define FABLE_RETAIL_STLP_TREE_ERASE_INLINE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CDEFENDINGCOMBATANTINFO_PRONG
#define FABLE_RETAIL_CDEFENDINGCOMBATANTINFO_WHEELINIT
#define FABLE_RETAIL_CCOMBATTYPEDEF
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCCOMBAT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_JACK_STAGE1_UPDATE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_COMBATTYPE
#define FABLE_RETAIL_CTCCOMBAT_UNSHEATHINGTYPE
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCombatTypeDef.h"
#include "engine/CAttackingCombatantInfo.h"
#include "engine/ECombatAbilityDataType.h"
#include "engine/CTCCombat.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCWeapon.h"
#include "engine/CDefendingCombatantInfo.h"

void CDefendingCombatantInfo::CCombatWheel::Init(CCombatManager&, CWorldMap& world,
                                                 const std::map<CThingCreatureBase*,CAttackingCombatantInfo*>& attackers,
                                                 const CThingCreatureBase& defender)
{
    std::map<CThingCreatureBase*,CAttackingCombatantInfo*> inner_attackers;
    std::map<CThingCreatureBase*,CAttackingCombatantInfo*> outer_attackers;
    std::list<CThingCreatureBase*> outer_attackers_list;
    const CCombatTypeDef& combat_type = defender.GetCombatTypeDef();
    for (std::map<CThingCreatureBase*,CAttackingCombatantInfo*>::const_iterator it = attackers.begin(); it != attackers.end(); ++it) {
        CThingCreatureBase* attacker = it->first;
        bool melee = false;
        const CTCCombat* combat;
        if (attacker->PeekTC(&combat) && combat->GetUnsheathingWeaponType() != UNSHEATHE_NULL) {
            if (combat->GetUnsheathingWeaponType() == UNSHEATHE_MELEE_WEAPON)
                melee = true;
        } else {
            const CTCCarrying* carrying;
            if (attacker->PeekTC(&carrying)) {
                CThing* weapon = carrying->GetThingInPrimarySlot();
                const CTCWeapon* weapon_tc;
                if (weapon && !weapon->IsDead() && weapon->PeekTC(&weapon_tc) && weapon_tc->IsMeleeWeapon())
                    melee = true;
            }
        }
        if (attacker->GetCombatTypeDef().PreferOuterRing) {
            outer_attackers.insert(*it);
            if (melee)
                outer_attackers_list.push_front(attacker);
            else
                outer_attackers_list.push_back(attacker);
        } else {
            inner_attackers.insert(*it);
        }
    }
    long num_to_move = 0;
    if (inner_attackers.size() == 0 && outer_attackers.size() != 0)
        num_to_move = 1;
    if (combat_type.DefenderWheelOuterRingStartIndex == 0)
        num_to_move = outer_attackers.size();
    for (long i = num_to_move; i > 0; --i) {
        std::list<CThingCreatureBase*>::iterator first = outer_attackers_list.begin();
        std::map<CThingCreatureBase*,CAttackingCombatantInfo*>::iterator found = outer_attackers.find(*first);
        inner_attackers.insert(*found);
        outer_attackers.erase(found);
        outer_attackers_list.erase(first);
    }
    for (std::vector<CCombatRing>::iterator ring = CombatRings.begin(); ring != CombatRings.end(); ++ring)
        ring->Reset();
    long num_rings = combat_type.DefenderCombatRingOccupationNumbers.size();
    long ring_index = 0;
    while (inner_attackers.size() != 0 && ring_index < num_rings) {
        CCombatRing* ring = GetRing(ring_index++);
        if (!ring)
            ring = &AddRing(combat_type);
        ring->AssignAttackersToRing(world, *this, inner_attackers, defender);
    }
    if (combat_type.DefenderWheelOuterRingStartIndex > ring_index)
        ring_index = combat_type.DefenderWheelOuterRingStartIndex;
    while (outer_attackers.size() != 0 && ring_index < num_rings) {
        CCombatRing* ring = GetRing(ring_index++);
        if (!ring)
            ring = &AddRing(combat_type);
        ring->AssignAttackersToRing(world, *this, outer_attackers, defender);
    }
    long total = CombatRings.size();
    if (ring_index < total) {
        long extra = total - ring_index;
        do {
            if (!CombatRings.empty())
                CombatRings.erase(CombatRings.end() - 1);
        } while (--extra);
    }
    if (inner_attackers.size() != 0 || outer_attackers.size() != 0) {
        for (std::map<CThingCreatureBase*,CAttackingCombatantInfo*>::iterator it = inner_attackers.begin(); it != inner_attackers.end(); ++it)
            it->second->CombatWheelRingIndex = -1;
        for (std::map<CThingCreatureBase*,CAttackingCombatantInfo*>::iterator it = outer_attackers.begin(); it != outer_attackers.end(); ++it)
            it->second->CombatWheelRingIndex = -1;
    }
}
