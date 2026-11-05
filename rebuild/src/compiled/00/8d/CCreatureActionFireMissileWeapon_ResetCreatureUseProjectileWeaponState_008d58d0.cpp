// CCreatureAction_FireMissileWeapon::ResetCreatureUseProjectileWeaponState() -- for a live creature, release any missile still held, then cancel the hero's weapon aiming (ranged-attacks component 0x81)
// Leaf: ?ResetCreatureUseProjectileWeaponState@CCreatureAction_FireMissileWeapon@@QAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D58D0, length 134 bytes, plain `ret`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CCreatureAction_FireMissileWeapon::ReleaseMissile
//          (CThing**, CThing**, long*) @ 0x008D5790, CVectorMap::LowerBound @ 0x0040F020,
//          CTCHeroRangedAttacks::CancelAimingWeapon @ 0x00789FF0.
// Shape: PCreature's address is taken once (esi); IS_THING_ALIVE inlined; ReleaseMissile gets the addresses of three
//   block-scoped locals (only the first is zeroed, after the pushes); the lookup key then reuses the dead first
//   local's frame word; the bool-returning out-parameter GetInterface for bit 0x81 (`test byte ptr [eax+30h],2`)
//   loads the component straight into ecx for the call.
// Layout: CCreatureActionBase (retail 0xa8 bytes) with PCreature at +8; nothing else of the class is touched.
class CTCBase;

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.Get() != 0) {
        if (!p.Get()->IsDead())
            return true;
    }
    return false;
}

class CTCHeroRangedAttacks { public:               // interface type 0x81 component
    void CancelAimingWeapon();              // 0x00789FF0
};

class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0xa8 - 0x10];
};

class CCreatureAction_FireMissileWeapon : public CCreatureActionBase { public:
    void ReleaseMissile(CThing** missile, CThing** target, long* globalIndex);   // 0x008D5790
    void ResetCreatureUseProjectileWeaponState();
};

void CCreatureAction_FireMissileWeapon::ResetCreatureUseProjectileWeaponState()
{
    if (IS_THING_ALIVE(PCreature)) {
        {
            CThing* missile = 0;
            CThing* target;
            long globalIndex;
            ReleaseMissile(&missile, &target, &globalIndex);
        }
        CTCHeroRangedAttacks* ranged;
        if (PCreature.Get()->GetTC(TCI_HERO_RANGED_ATTACKS, &ranged))
            ranged->CancelAimingWeapon();
    }
}