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
#if defined(FABLE_RETAIL_CENGINEDEF_LODERRORTOLERANCE) && !defined(FABLE_RETAIL_CENGINEDEF_FOV_2D)
    // Opt-in (wfE7, row 0x00B33B50): Ego_r.pdb CEngineDef::LODErrorTolerance / CharacterLODErrorTolerance (+0x40 / +0x44 there, retail
    // +0x3C / +0x40: the -4 container shift as FOV_2D), read by the CEnginePrimitiveRenderer ctor (0x00B35CF2 / 0x00B35D02).
private:
    unsigned char _pad_0x25[0x3c - 0x25];
public:
    float LODErrorTolerance;                                             // +0x3c
    float CharacterLODErrorTolerance;                                    // +0x40
private:
    unsigned char _pad_0x44[0x80 - 0x44];
#elif defined(FABLE_RETAIL_CENGINEDEF_FOV_2D)
    // Opt-in (wf21, row 0x00B4AC10): Ego_r.pdb CEngineDef::FOV_2D (+0x60 there, retail +0x5c: the -4 container shift), read by the 2D renderer ctor.
private:
    unsigned char _pad_0x25[0x37];                                       // +0x25 members not modelled yet
public:
    float FOV_2D;                                                        // +0x5c
private:
    unsigned char _pad_0x60[0x20];                                       // +0x60 members not modelled yet
#elif defined(FABLE_RETAIL_CENGINEDEF_DRAW_DISTANCE_PASS27)
private:
    unsigned char _pad_0x25[0x50 - 0x25];
public:
    float GamePrimitiveDefaultFadeRangeRatio;                            // +0x50, Ego_r offset 0x54
private:
    unsigned char _pad_0x54[0x80 - 0x54];
#else
private:
    unsigned char _pad_0x25[0x5b];                                       // +0x25 members not modelled yet
#endif
#ifdef FABLE_RETAIL_CENGINEDEF_ALLOC
// Opt-in (wfE11, 2026-11-15, row 0x0044C72B CGameDefinitionManager::InitAndCompile, which registers its address): the static allocator
// ?Alloc@CEngineDef@@SIPAVCDefClassBase@@XZ (Ego_r.pdb + FableWin.pdb). Declaration only (elaborated `class`: needs no include); no layout change.
public: static class CDefClassBase* FABLE_FASTCALL Alloc();
#endif
};
FABLE_STATIC_ASSERT_AT(24, sizeof(CEngineDef) == 0x80);
#endif // FABLE_ENGINE_CENGINEDEF_H
