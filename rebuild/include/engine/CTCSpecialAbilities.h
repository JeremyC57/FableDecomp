#ifndef FABLE_ENGINE_CTCSPECIALABILITIES_H
#define FABLE_ENGINE_CTCSPECIALABILITIES_H
// New (chunk 1012, row 0x0062BEF0 CWorld::EAUseProjectileWeapon): the hero special-abilities component, interface TCI_SPECIAL_ABILITIES
// (167; RTTI .?AVCTCSpecialAbilities@@ : CTCBase). Ego_r.pdb CTCSpecialAbilities (0x38 bytes there): AbilityMutexLock +0x0C, UsingBed
// +0x0D, CurrentAbility +0x10, UndeadFollowers (STLport list, 4 bytes in retail) +0x14, Creature ...; only the first byte is read by
// the rows that use this header, through FableWin.pdb's inline ?GetAbilityMutexLock@CTCSpecialAbilities@@QBE_NXZ (retail
// `mov cl,[eax+0Ch] ; test cl,cl`, 0x0062C001). The rest of the class stays unmodelled. Needs FABLE_RETAIL_CTCBASE.
#ifdef FABLE_RETAIL_CTCSPECIALABILITIES
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include "rebuild_abi.h"
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
class CTCSpecialAbilities : public CTCBase {
public:
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_SPECIAL_ABILITIES; }
    bool GetAbilityMutexLock() const { return AbilityMutexLock; }
private:
    bool AbilityMutexLock;                                   // +0x0C
    bool UsingBed;                                           // +0x0D
};
#else
class CTCSpecialAbilities;
#endif
#endif // FABLE_ENGINE_CTCSPECIALABILITIES_H
