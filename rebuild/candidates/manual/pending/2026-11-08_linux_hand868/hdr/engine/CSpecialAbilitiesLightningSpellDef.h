#ifndef FABLE_ENGINE_CSPECIALABILITIESLIGHTNINGSPELLDEF_H
#define FABLE_ENGINE_CSPECIALABILITIESLIGHTNINGSPELLDEF_H
// CSpecialAbilitiesLightningSpellDef (chunk 868, row 0x007A1B20): only the sound ids the will-lightning rows read are modelled.
// Ego_r.pdb order InitSound +0x154, LoopingSound +0x158, EndHeroSound +0x15c, EndAttackeeSound +0x160 (PDB sizeof 0x16c); the retail
// def is 0x44 smaller before them (STLport containers): the landed CTCWillLightning ctor / StartLoopingSound 0x007A0400 read InitSound
// at +0x110 and LoopingSound at +0x114, so EndHeroSound is +0x118 and EndAttackeeSound +0x11c (0x007A1B20 `mov ecx,[eax+11Ch]`).
// Needs FABLE_RETAIL_CDEFSTRINGTABLE + FABLE_RETAIL_CCHARSTRING (engine/CDefString.h).
#include "rebuild_abi.h"
#include "engine/CDefString.h"
#include "engine/CDefPointeeBase.h"
#pragma pack(push, 1)
class CSpecialAbilitiesLightningSpellDef : public CDefPointeeBase {   // the def-class chain (CDefClassBase ...) is not modelled: only the pointee base CDefPointer needs
public:
    unsigned char _pad_base[0x110 - sizeof(CDefPointeeBase)];  // +sizeof(CDefPointeeBase) .. the rest of the def-class base and the spell parameters (not modelled)
    CDefString    InitSound;                                 // +0x110
    CDefString    LoopingSound;                              // +0x114
    CDefString    EndHeroSound;                              // +0x118
    CDefString    EndAttackeeSound;                          // +0x11c
};
#pragma pack(pop)
FABLE_STATIC_ASSERT(offsetof(CSpecialAbilitiesLightningSpellDef, EndAttackeeSound) == 0x11c);
#endif
