// CCreatureAction_GenericLoadProjectileWeapon::FinishAction() -- if the creature carries a live weapon with a weapon component (0x48), mark its projectile as loaded; then the base FinishAction
// Leaf: ?FinishAction@CCreatureAction_GenericLoadProjectileWeapon@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008BF6A0, length 145 bytes, plain `ret`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound @ 0x0040F020 (x2),
//          CTCCarrying::GetThingInPrimarySlot @ 0x006A1440, CTCWeapon::SetProjectileWeaponLoaded(bool, CThing*)
//          @ 0x005DAC50, CCombatActionBase::FinishAction @ 0x00859A20.
// Shape: the carrying component (0x46) comes from the bool-returning out-parameter GetInterface template; the weapon
//   component (0x48) is a HasTC test (flags dword loaded into ecx, `test ch,1`) plus a pointer-returning lookup whose
//   result goes straight into ecx for the call (the out-parameter form parks it in eax first); the carried thing's alive test is inline on the raw pointer (null, then dead bit +0x91);
//   everything joins on the base FinishAction call.
// Layout: CCombatActionBase (retail 0x114 bytes) with PCreature at +8; nothing else of the class is touched.
class CTCBase;

#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


class CTCCarrying { public:                        // interface type 0x46 component
public:
    CThing* GetThingInPrimarySlot() const;   // 0x006A1440
};

class CTCWeapon { public:                          // interface type 0x48 component
    void SetProjectileWeaponLoaded(bool loaded, CThing* projectile);   // 0x005DAC50
};

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];
    protected: virtual void FinishAction(); public:            // 0x00859A20
};

class CCreatureAction_GenericLoadProjectileWeapon : public CCombatActionBase { public:
    virtual void FinishAction();
};

void CCreatureAction_GenericLoadProjectileWeapon::FinishAction()
{
    CTCCarrying* carrying;
    if (PCreature.Get()->GetTC(TCI_CARRYING, &carrying)) {
        CThing* weapon = carrying->GetThingInPrimarySlot();
        if (weapon != 0 && !weapon->IsDead()) {
            if (weapon->HasTC(TCI_WEAPON))
                ((CTCWeapon*)((CTCWeapon*)&weapon->GetTC(TCI_WEAPON)))->SetProjectileWeaponLoaded(true, 0);
        }
    }
    CCombatActionBase::FinishAction();
}