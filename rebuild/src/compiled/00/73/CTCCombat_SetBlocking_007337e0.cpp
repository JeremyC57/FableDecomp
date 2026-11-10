// CTCCombat::SetBlocking() -- enter the blocking state: a ranged-attacks user stops aiming and resets its projectile weapon mode (and its player drops mode 0x22); the creature mode manager is put into mode 0xc unless already there; Blocking is set
// Leaf: ?SetBlocking@CTCCombat@@QAEXXZ
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007337E0, 182 bytes (plain `ret`).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (two inlined lookups), CTCHeroRangedAttacks::
// CancelAimingWeapon @ 0x00789FF0 and ::ResetProjectileWeaponMode @ 0x00789B30, CThing::GetPlayer @ 0x004C7A30, CPlayer::RemoveMode
// @ 0x00634190, CTCCreatureModeManager::GetCurrentMode @ 0x0073A6E0 (manifest "CPixelShader::CountReferences" is propagated) and
// ::AddMode @ 0x0073AE10, the mode's virtual slot 1 GetModeType.
// Layout: local model -- engine/CTCCombat.h carries Ego_r.pdb offsets (Blocking +0x6f) while retail's three 12-byte vectors put
// Blocking at +0x63, and the header's +0x04 word is the CTCBase ParentThing. CThing bitset +0x20 (ranged attacks = 0x81, bit 1 of
// the +0x30 word; mode manager = 0x31, bit 17 of the +0x24 word), map +0x44.
// Shape: HasTC + GetTC with the ranged-attacks component in a named local (esi); the mode manager is the unchecked out-parameter
// GetInterface (the absent arm reads the lookup key's frame word); RemoveMode's argument is pushed before GetPlayer runs.
// Name fix (agentNF5 2026-11-10): CPlayer::RemoveMode 0x00634190 takes EPlayerMode (?RemoveMode@CPlayer@@QAEXW4EPlayerMode@@@Z, Ego_r.pdb / FableWin.pdb), was long.
enum EPlayerMode;                                  // MS forward enum (CPlayer::RemoveMode 0x00634190 takes it)
class CTCBase;
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CPlayer { public:
    void RemoveMode(EPlayerMode mode);                         // 0x00634190
};

class CTCHeroRangedAttacks { public:
    void CancelAimingWeapon();                          // 0x00789FF0
    void ResetProjectileWeaponMode();                   // 0x00789B30
};
struct CMode {
    virtual void v00();
    virtual long GetModeType();                         // slot 1
};
class CTCCreatureModeManager { public:
    CMode* GetCurrentMode();                            // 0x0073A6E0
    bool AddMode(unsigned long mode);                   // 0x0073AE10
};
class CTCCombat { public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    unsigned char _pad08[0x63 - 0x08];
    bool Blocking;                                      // +0x63 (PDB +0x6f)
    void SetBlocking();
};

void CTCCombat::SetBlocking()
{
    if (ParentThing->HasTC(TCI_HERO_RANGED_ATTACKS)) {
        CTCHeroRangedAttacks* ranged = (CTCHeroRangedAttacks*)((CTCHeroRangedAttacks*)&ParentThing->GetTC(TCI_HERO_RANGED_ATTACKS));
        ranged->CancelAimingWeapon();
        ranged->ResetProjectileWeaponMode();
        ParentThing->GetPlayer().RemoveMode((EPlayerMode)(0x22));
    }
    CTCCreatureModeManager* modes;
    ParentThing->GetTC(TCI_ENTITY_MODE_MANAGER, &modes);
    if (modes->GetCurrentMode()->GetModeType() != 0xc)
        modes->AddMode(0xc);
    Blocking = true;
}