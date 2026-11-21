// CCombatManager::DeregisterDestroyedCombatant(CThingCreatureBase* creature) -- take a destroyed creature out of combat: as an attacker (telling its target's stealth component that the danger is over) and as an attacked combatant
// Leaf: ?DeregisterDestroyedCombatant@CCombatManager@@QAEXPAVCThingCreatureBase@@@Z
// Name fix (NF96 part 2, q11460): the local CTCStealth declared SetAsInDanger (CThing* source, EDangerStatus status) and bound a stand-in spelling of 0x00747B80;
//   now the Ego_r.pdb + FableWin.pdb ?SetAsInDanger@CTCStealth@@QAEXABVCThing@@W4EDangerStatus@NStealth@@@Z (engine/CTCStealth.h signature;
//   the reference and the enum push the same words).
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) @ 0x006ED9E0, 148 bytes (160 with padding).
// Callees: CThingCreatureBase::IsInMeleeAsAttacker 0x00662410 and IsInMeleeAsAttacked 0x00662430, the combatant map's find @ 0x006F0250, CCombatManager::DeregisterAttackingCombatant
//          0x006ED510 / DeregisterAttackedCombatant 0x006ED8C0, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
//          0x0040F020, CTCStealth::SetAsInDanger(CThing*, EDangerStatus) 0x00747B80.
//
// Shape: the landed CCombatManager model (Combatants at +0x10, node value at +0x18, AttackerInfo.Attacked at
// value +4) and the landed GetInterface out-parameter template (CThingNoise::PassToNoiseComponent 0x008386E0):
// interface 0x36 (stealth) is bit 22 of the bitset word at +0x24, the interface map at +0x44 and the
// lookup key homed in the dead `creature` argument slot. The target is read from the combatant info
// BEFORE the attacker is deregistered (the node may go away); the 64-bit id is a local copied into the
// frame high word first. The template is `__forceinline`: as a plain inline member VC7.1 emitted it
// out of line in this TU (call + `push 0x36`).
// Name fix (agentNF6 2026-11-10): CThingCreatureBase::IsInMeleeAsAttacker 0x00662410 is ?IsInMeleeAsAttacker@CThingCreatureBase@@QBE_NXZ (Ego_r.pdb / FableWin.pdb), was IsAttackingCombatant / IsInCombatAsAttacker / IsRegisteredCombatant.
// Name fix (NF199, 2026-11-20, q4389): the invented CCombatantMap / SCombatantNode / CCombatantInfo_Retail stand-ins are replaced by
//   the STLport map<unsigned __int64, CCombatantInfo> (stlport/fable_map.h) over the shared engine/CCombatantInfo.h (retail 0x70 bytes,
//   AttackerInfo.Attacked at +4); find's out-of-line _M_find<unsigned __int64> 0x006F0250 is declared only
//   (FABLE_RETAIL_STLP_TREE_MFIND_DECL), so the call binds the self row's member-template spelling.
// Name fix (NF215, 2026-11-21, q12339): 0x00662430 was the stand-in CThingCreatureBase::IsCombatant() (?IsCombatant@CThingCreatureBase@@QAE_NXZ);
//   it is Ego_r.pdb (0x0053D2C0) + FableWin.pdb (0x01DA3370) ?IsInMeleeAsAttacked@CThingCreatureBase@@QBE_NXZ (the self row's Leaf), respelled
//   in place in the row's existing local model as NF211 did 0x006ED510.
class CTCBase;

#define FABLE_RETAIL_STLP_TREE_MFIND_DECL
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingCreatureBase : public CThing { public:
    bool IsInMeleeAsAttacker() const;                // 0x00662410
    bool IsInMeleeAsAttacked() const;           // 0x00662430
};

namespace NStealth {   // Ego_d.pdb LF_ENUM NStealth::EDangerStatus (the enumerators of engine/CTCStealth.h FABLE_RETAIL_NSTEALTH_EDANGERSTATUS)
enum EDangerStatus { DANGER_STATUS_NULL=0, DANGER_STATUS_INVESTIGATING=1, DANGER_STATUS_CHASING=2, DANGER_STATUS_COMBAT=3, DANGER_STATUS_RANGED_COMBAT=4 };
}

class CTCStealth { public:                             // interface type 0x36
    void SetAsInDanger(const CThing& source, NStealth::EDangerStatus status);   // 0x00747B80
};

#include "engine/CCombatantInfo.h"

class CCombatManager { public:
    void*         __vftable;                    // +0x00
    unsigned char RegisteredCombatSequences[0xc];   // +0x04
    std::map<unsigned __int64, CCombatantInfo> Combatants;   // +0x10
    private: void DeregisterAttackingCombatant(CThingCreatureBase& creature); public:   // 0x006ED510 (Ego_r.pdb + FableWin.pdb, private)
    void DeregisterAttackedCombatant(CThingCreatureBase* creature);    // 0x006ED8C0
    void DeregisterDestroyedCombatant(CThingCreatureBase* creature);
};

void CCombatManager::DeregisterDestroyedCombatant(CThingCreatureBase* creature)
{
    if (creature->IsInMeleeAsAttacker()) {
        unsigned __int64 id = creature->GetUniqueID();
        CThingCreatureBase* attacked = Combatants.find(id)->second.AttackerInfo.Attacked;
        DeregisterAttackingCombatant(*creature);
        CTCStealth* stealth;
        if (attacked->GetTC(TCI_STEALTH, &stealth))
            stealth->SetAsInDanger(*creature, NStealth::DANGER_STATUS_NULL);
    }
    if (creature->IsInMeleeAsAttacked())
        DeregisterAttackedCombatant(creature);
}