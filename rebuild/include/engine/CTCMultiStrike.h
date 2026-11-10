#ifndef FABLE_ENGINE_CTCMULTISTRIKE_H
#define FABLE_ENGINE_CTCMULTISTRIKE_H
// CTCMultiStrike (new shared header, chunk 1004, row 0x005CA68E CTCInventoryWeapons::SetupActiveWeaponsOnCarrier): the multi-strike
// spell component over FABLE_RETAIL_CTCBASE (Ego_r.pdb sizeof 0x5C: PDef +0x0C, SpellLevel +0x10, StrikeStarted +0x14 .. ParticleEffects
// +0x4C; members not modelled). StatGetInterfaceType: TCI_MULTI_STRIKE (135; the `mov edi,87h` of the caller's inline GetTC, 0x005CA6F3).
// Out-of-line methods the rows call (Ego_r.pdb + FableWin.pdb spellings): ?ApplyEffectOnWeapon@CTCMultiStrike@@QAEXPAVCThing@@@Z
// (retail 0x007CCF50) and ?RemoveEffectOnWeapon@CTCMultiStrike@@QAEXPAVCThing@@@Z (0x007CC9E0). Declarations only.
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
class CThing;
class CTCMultiStrike : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_MULTI_STRIKE; }
    void ApplyEffectOnWeapon(CThing* weapon);                // 0x007CCF50
    void RemoveEffectOnWeapon(CThing* weapon);               // 0x007CC9E0
};
#endif // FABLE_ENGINE_CTCMULTISTRIKE_H
