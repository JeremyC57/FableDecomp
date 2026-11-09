#ifndef FABLE_ENGINE_CENGINEPRIMITIVE2DVIEWPORT_H
#define FABLE_ENGINE_CENGINEPRIMITIVE2DVIEWPORT_H
// New shared header (chunk 892, row 0x00BE1C10): Ego_r.pdb CEnginePrimitive2DViewport (the 2D viewport primitive description,
// sizeof 0x48) : CEnginePrimitive. 0x00BE1C10 reads Layer +0x0C, Viewport +0x10, the FOVs +0x20 / +0x24, Use2DFOV +0x28,
// AlphaMaskBank +0x2C, AlphaMaskBankIndex +0x30, AlphaMaskClipBox +0x34, MaskObjectsOutsideAlphaMask +0x44, FilterMask +0x45.
#include "rebuild_abi.h"
#include "engine/CEnginePrimitive.h"
#include "engine/C2DBoxF.h"
class CGraphicDataBank;
enum EEngine2DLayer;
class CEnginePrimitive2DViewport : public CEnginePrimitive {
public:
    EEngine2DLayer    Layer;                        // +0x0c
    C2DBoxF           Viewport;                     // +0x10
    float             HorizontalFOV;                // +0x20
    float             VerticalFOV;                  // +0x24
    bool              Use2DFOV;                     // +0x28
    CGraphicDataBank* AlphaMaskBank;                // +0x2c
    unsigned long     AlphaMaskBankIndex;           // +0x30
    C2DBoxF           AlphaMaskClipBox;             // +0x34
    bool              MaskObjectsOutsideAlphaMask;  // +0x44
    bool              FilterMask;                   // +0x45
};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitive2DViewport) == 0x48);
#endif
