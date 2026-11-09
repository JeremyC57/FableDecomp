#ifndef FABLE_ENGINE_C2DALPHAMASKINFO_H
#define FABLE_ENGINE_C2DALPHAMASKINFO_H
// New shared header (chunk 892, row 0x00BE1C10): Ego_r.pdb C2DAlphaMaskInfo, sizeof 0x2C, no STLport member (Ego_r offsets are
// retail's: 0x00BE1C10 writes AlphaMaskBank +0x64-0x64, the position factors +0x08, the clip box +0x18, the flags +0x28 / +0x29 of
// the CEngineInternalPrimitive2DViewport member at +0x64).
#include "rebuild_abi.h"
#include "engine/C2DBoxF.h"
#include "engine/C4DVector.h"
class CGraphicDataBank;
class C2DAlphaMaskInfo {
public:
    CGraphicDataBank* AlphaMaskBank;                // +0x00
    unsigned long     AlphaMaskBankIndex;           // +0x04
    C4DVector         AlphaMaskPositionFactors;     // +0x08
    C2DBoxF           AlphaMaskClipBox;             // +0x18
    bool              MaskObjectsOutsideAlphaMask;  // +0x28
    bool              FilterMask;                   // +0x29
};
FABLE_STATIC_ASSERT(sizeof(C2DAlphaMaskInfo) == 0x2C);
#endif
