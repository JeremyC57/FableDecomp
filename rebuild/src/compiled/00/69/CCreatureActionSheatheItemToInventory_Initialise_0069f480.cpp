// CCreatureAction_SheatheItemToInventory::Initialise() -- base Initialise, then if the creature has hero ranged attacks (interface type 0x81): cancel aiming, reset the projectile weapon mode and drop player mode 0x22
// Leaf: ?Initialise@CCreatureAction_SheatheItemToInventory@@UAEXXZ
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069F480, length 110 bytes (+ int3 padding).
// Callees: CCreatureActionBase::Initialise @ 0x00694D10 (direct base call, first), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8,
// twice), CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookup for type 0x81 = bit 1 of the bitset dword at +0x30, `test byte ptr`),
// CTCHeroRangedAttacks::CancelAimingWeapon @ 0x00789FF0, CTCHeroRangedAttacks::ResetProjectileWeaponMode @ 0x00789B30, CThing::GetPlayer @ 0x004C7A30,
// CPlayer::RemoveMode(EPlayerMode) @ 0x00634190.
// Shape: the out-parameter GetInterface as the if condition; the component rides in esi (re-used after `this` is dead) for both of its calls,
// &PCreature in edi. The PlayerMode argument is pushed before the creature/player chain (a chained call evaluates its argument first).
// No usable engine header layout (retail CCreatureActionBase is 0xa8 bytes); modelled like the landed _ForEnflame relative 0x0069F750.
class CTCBase;
class CPlayer { public:
    void RemoveMode(long mode);             // 0x00634190 (EPlayerMode by value)
};
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CTCHeroRangedAttacks { public:               // interface type 0x81 component
    void CancelAimingWeapon();              // 0x00789FF0
    void ResetProjectileWeaponMode();       // 0x00789B30
};

class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void Initialise();              // slot 12, 0x00694D10
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0xa8 - 0x10];      // +0x10
};
class CCreatureAction_SheatheItemToInventory : public CCreatureActionBase { public:
    virtual void Initialise();
};

void CCreatureAction_SheatheItemToInventory::Initialise()
{
    CCreatureActionBase::Initialise();
    CTCHeroRangedAttacks* pRanged;
    if (PCreature.Get()->GetTC(TCI_HERO_RANGED_ATTACKS, &pRanged)) {
        pRanged->CancelAimingWeapon();
        pRanged->ResetProjectileWeaponMode();
        PCreature.Get()->GetPlayer().RemoveMode(0x22);
    }
}