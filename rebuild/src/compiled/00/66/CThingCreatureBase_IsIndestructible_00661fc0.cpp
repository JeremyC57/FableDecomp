// CThingCreatureBase::IsIndestructible() const -- the creature def's IsIndestructible flag
// Leaf: ?IsIndestructible@CThingCreatureBase@@QBE_NXZ
// Retail: Fable.exe @ 0x00661FC0, 16 bytes, thiscall plain `ret`, bool in al.
// Identity (proven): vtable slot 67 (+0x10C) of CThingCreatureBase 0x0125AC0C, CThingPlayerCreature 0x012457FC and
// CThingAICreature 0x0127293C alike (rtti-vtables.tsv) -- an inherited CThingCreatureBase virtual, not a CThingPlayerCreature
// override. Ego_r.pdb has ?IsIndestructible@CThingCreatureBase@@UBE_NXZ and ?IsWillable@CThingCreatureBase@@UBE_NXZ
// adjacent (0x0053CE10 / 0x0053CE20), as retail has this body and the landed slot-75 body 0x00661FD0 (bit 6) adjacent; Ego_r.pdb
// CThingCreatureDef packs LeaveDeadCreature .. UnsheatheWeaponsAutomatically, IsWillable (bit 6), IsIndestructible (bit 7)
// into one byte (PDB +0x150, retail +0x148): this reads bit 7, 0x00661FD0 bit 6.
// Label correction: the catalog's "CThingPlayerCreature::vfunc_67" names the class by one of its three vtables; 0x00661FD0
// ("CThingPlayerCreature::vfunc_75") is CThingCreatureBase::IsWillable by the same evidence.
// Callees: none.
// Shape: the def pointer used in place (inline CDefPointer::operator->), the top bit as `shr al,7`.
// Layout: engine/CThingCreatureBase.h (PDef_Object +0xE0, retail STLport layout) through a donor include; that header carries
// the vptr as a data word, so the slot-67 virtual is declared as a plain member here (same code, `QBE` instead of `UBE`).
// No engine header for CThingCreatureDef: local partial view of the flag byte.
#define CThingCreatureBase CThingCreatureBaseDonor
#include "engine/CThingCreatureBase.h"
#undef CThingCreatureBase

class CThingCreatureDef {                                   // partial view, retail offsets
public:
    unsigned char _pad000[0x148];
    bool LeaveDeadCreature : 1;                             // +0x148 bit 0
    bool AbleToStrafe : 1;
    bool CreateDummyOnDeath : 1;
    bool ImmuneToAttack : 1;
    bool UseMeleeWeaponAsDefault : 1;
    bool UnsheatheWeaponsAutomatically : 1;
    bool IsWillable : 1;                                    // bit 6
    bool IsIndestructible : 1;                              // bit 7
};

class CThingCreatureBase : public CThingCreatureBaseDonor {
public:
    const CThingCreatureDef* PeekDef() const { return (const CThingCreatureDef*)PDef_Object; }
    bool IsIndestructible() const;                          // virtual slot 67 in retail
};

bool CThingCreatureBase::IsIndestructible() const
{
    return PeekDef()->IsIndestructible;
}