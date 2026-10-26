// CThingCreatureBase::IsInMeleeAsAttacked() const -- ask the world's combat manager whether this creature is being attacked
// Leaf: ?IsInMeleeAsAttacked@CThingCreatureBase@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00662430, length 21 bytes (+ int3 padding): `push esi ; mov esi,ecx ;
//   call 0x004C7AB0 ; mov eax,[eax+4Ch] ; push esi ; mov ecx,eax ; call 0x006E9270 ; pop esi ; ret`.
// Identity (FINDINGS chunk 407 callee "0x662410/0x662430 IsInMeleeAsAttacker/IsInMeleeAsAttacked"; labels_chunk282
//   already; was the void stand-in CThingCreatureBase::IsCombatant): Ego_r.pdb `?IsInMeleeAsAttacked@CThingCreatureBase@@QBE_NXZ` (bool, const);
//   the body is the inline CThingCreatureBase::GetCombatManager() const (`?GetCombatManager@CThingCreatureBase@@QBE
//   ABVCCombatManager@@XZ`) over CThing::GetWorld 0x004C7AB0 (`?GetWorld@CThing@@IAEAAVCWorld@@XZ`; the world from the
//   global thing manager) and CWorld::PThingCombatManager (scoped_ptr<CCombatManager>, +0x4C in struct_layouts and
//   engine/CWorld.h), then CCombatManager::IsAttacked(const CThingCreatureBase&) const 0x006E9270 (Ego_r.pdb
//   `?IsAttacked@CCombatManager@@QBE_NABVCThingCreatureBase@@@Z`; renamed in this pass) on *this; the bool result rides
//   through in al.
#include "engine/CWorld.h"

class CThingCreatureBase;

class CCombatManager { public:
    bool IsAttacked(const CThingCreatureBase& creature) const;   // 0x006E9270
};

class CThing {
protected:
    CWorld& GetWorld();                          // 0x004C7AB0
};

class CThingCreatureBase : public CThing {
public:
    const CCombatManager& GetCombatManager() const
    {
        return *const_cast<CThingCreatureBase*>(this)->GetWorld().PThingCombatManager_ptr;
    }
    bool IsInMeleeAsAttacked() const;
};

bool CThingCreatureBase::IsInMeleeAsAttacked() const
{
    return GetCombatManager().IsAttacked(*this);
}
