#ifndef FABLE_ENGINE_CLIGHTNINGORBDEF_H
#define FABLE_ENGINE_CLIGHTNINGORBDEF_H
// New shared header (w22 agent2022, row 0x007F26B0 CTCLightningOrb::Initialise): the lightning-orb definition (the generated flattened
// copy sits in engine/_quarantine/). RTTI .?AVCLightningOrbDef@@ : CSubDefClassBase (ghidra_out/rtti_bases.tsv); Ego_r.pdb / FableWin.pdb
// Transfer / Copy / GetSizeofClass overrides and the static StatGetTypeName. Members: Ego_r.pdb names in order at the Ego_r offsets (no
// container: CSubDefClassBase is 0x25 bytes in retail, so the first long sits at +0x28 as in Ego_r): LightningDefIndex +0x28 (Initialise
// 0x007F2BCC), LifeTime +0x2C, Speed +0x30 (Initialise 0x007F2BA7 / 0x007F2BBB), ExplosionRange +0x34 / ExplosionDefIndex +0x38 (the
// landed FrameUpdate 0x007F24E0); sizeof 0x3C. Needs FABLE_RETAIL_DEF_CLASS_BASE.
#include <stddef.h>
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CSubDefClassBase.h"
class CPersistContext;
class CDefClassBase;

class CLightningOrbDef : public CSubDefClassBase {
public:
    static CCharString __fastcall StatGetTypeName() { return CCharString("CLightningOrbDef", -1); }   // ?StatGetTypeName@CLightningOrbDef@@SI?AVCCharString@@XZ
    virtual void Transfer(CPersistContext&);                     // ?Transfer@CLightningOrbDef@@UAEXAAVCPersistContext@@@Z
    virtual void Copy(const CDefClassBase*);                     // ?Copy@CLightningOrbDef@@UAEXPBVCDefClassBase@@@Z
    virtual unsigned long GetSizeofClass();                      // ?GetSizeofClass@CLightningOrbDef@@UAEKXZ
    long  LightningDefIndex;                                     // +0x28
    float LifeTime;                                              // +0x2c
    float Speed;                                                 // +0x30
    float ExplosionRange;                                        // +0x34
    long  ExplosionDefIndex;                                     // +0x38
};
FABLE_STATIC_ASSERT(offsetof(CLightningOrbDef, LightningDefIndex) == 0x28);
FABLE_STATIC_ASSERT(offsetof(CLightningOrbDef, Speed) == 0x30);
FABLE_STATIC_ASSERT(sizeof(CLightningOrbDef) == 0x3c);
#endif
