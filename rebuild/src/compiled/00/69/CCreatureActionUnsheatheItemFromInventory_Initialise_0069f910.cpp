// CCreatureAction_UnsheatheItemFromInventory::Initialise() -- base Initialise; if the creature has the hero ranged-attacks component (interface type 0x81) cancel its aiming and reset its projectile mode, then take the creature's player out of mode 0x22
// Leaf: ?Initialise@CCreatureAction_UnsheatheItemFromInventory@@UAEXXZ
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069F910, length 110 bytes.
// Callees: CCreatureActionBase::Initialise @ 0x00694D10 (direct base call), CBaseIntelligentPointer::GetPItem @ 0x00A01B50
// (PCreature at +8, twice), CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookup for type 0x81 = bit 1 of the
// bitset dword at +0x30, `test byte ptr [eax+30h],2`), CTCHeroRangedAttacks::CancelAimingWeapon @ 0x00789FF0,
// CTCHeroRangedAttacks::ResetProjectileWeaponMode @ 0x00789B30, CThing::GetPlayer @ 0x004C7A30, CPlayer::RemoveMode(EPlayerMode)
// @ 0x00634190.
// Shape: the &PCreature address stays in edi (named by the compiler, not the source); the component rides in esi for its two
// calls; the player chain pushes 0x22 before evaluating its object expression (natural VC argument-first order). Retail
// CCreatureActionBase is 0xa8 bytes (the PDB build is larger), so `this` is modelled locally.
// Name fix (agentNF5 2026-11-10): CPlayer::RemoveMode 0x00634190 takes EPlayerMode (?RemoveMode@CPlayer@@QAEXW4EPlayerMode@@@Z, Ego_r.pdb / FableWin.pdb), was long.
enum EPlayerMode;                                  // MS forward enum (CPlayer::RemoveMode 0x00634190 takes it)
class CTCBase;
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CPlayer { public:
    void RemoveMode(EPlayerMode mode);             // 0x00634190 (EPlayerMode)
};

class CTCHeroRangedAttacks { public:
    void CancelAimingWeapon();              // 0x00789FF0
    void ResetProjectileWeaponMode();       // 0x00789B30
};
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


class CCreatureActionBase { public:                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0xa8 - 0x10];      // +0x10
public:
    virtual void Initialise();   // 0x00694D10
};

class CCreatureAction_UnsheatheItemFromInventory : public CCreatureActionBase { public:
    void Initialise();
};

void CCreatureAction_UnsheatheItemFromInventory::Initialise()
{
    CCreatureActionBase::Initialise();
    CTCHeroRangedAttacks* ranged;
    if (PCreature.Get()->GetTC(TCI_HERO_RANGED_ATTACKS, &ranged)) {
        ranged->CancelAimingWeapon();
        ranged->ResetProjectileWeaponMode();
        PCreature.Get()->GetPlayer().RemoveMode((EPlayerMode)(0x22));
    }
}