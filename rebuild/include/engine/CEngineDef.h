#ifndef FABLE_ENGINE_CENGINEDEF_H
#define FABLE_ENGINE_CENGINEDEF_H
// New header (chunk 731, 2026-11-03; one model, no class macro): the def class CEngineDef : CDefClassBase (Ego_r.pdb / FableWin.pdb
// ??0/??_E/Alloc/Copy/GetSizeofClass spellings). sizeof 0x80: retail ?GetSizeofClass@CEngineDef@@UAEKXZ 0x00433FD0 returns it
// (`mov eax,0x80 ; ret`); scalar deleting destructor 0x004340B0 (vtable store in the scalar deleting dtor), Copy 0x00434360. CEngine::Initialise 0x00B26360 holds it through
// CDefPointer<const CEngineDef> and publishes the raw pointer. Own members are not modelled yet (one opaque block after the packed
// 0x25-byte CDefClassBase; the quarantined generated model engine/_quarantine/CEngineDef.h lists the Ego_r names).
#include "rebuild_abi.h"
#include "engine/CDefClassBase.h"
class CEngineDef : public CDefClassBase {
#ifdef FABLE_RETAIL_CENGINEDEF_FOV_2D
    // Opt-in (wf21, row 0x00B4AC10): Ego_r.pdb CEngineDef::FOV_2D (+0x60 there, retail +0x5c: the -4 container shift), read by the 2D renderer ctor.
private:
    unsigned char _pad_0x25[0x37];                                       // +0x25 members not modelled yet
public:
    float FOV_2D;                                                        // +0x5c
private:
    unsigned char _pad_0x60[0x20];                                       // +0x60 members not modelled yet
#else
private:
    unsigned char _pad_0x25[0x5b];                                       // +0x25 members not modelled yet
#endif
};
FABLE_STATIC_ASSERT(sizeof(CEngineDef) == 0x80);
#endif // FABLE_ENGINE_CENGINEDEF_H
