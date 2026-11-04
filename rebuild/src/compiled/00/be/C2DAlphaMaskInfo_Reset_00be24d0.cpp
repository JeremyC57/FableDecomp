// C2DAlphaMaskInfo::Reset() -- clear the alpha-mask info: no bank, index 0, zero position factors, both flags off
// Leaf: ?Reset@C2DAlphaMaskInfo@@QAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BE24D0, 54 bytes, thiscall, plain `ret`.
// Identity: Ego_r.pdb / FableWin.pdb ?Reset@C2DAlphaMaskInfo@@QAEXXZ; the class is the PDB C2DAlphaMaskInfo (0x2C bytes: AlphaMaskBank +0x00,
//   AlphaMaskBankIndex +0x04, C4DVector AlphaMaskPositionFactors +0x08, C2DBoxF AlphaMaskClipBox +0x18,
//   MaskObjectsOutsideAlphaMask +0x28, FilterMask +0x29), in the engine 2D-viewport TU (0x00BE2xxx). The catalog's
//   "CVO_OracleMarker::Init" was a propagated label. No engine header models the class yet (a local class with the PDB layout).
// Shape: the clip box is left alone; `AlphaMaskPositionFactors = C4DVector(0, 0, 0, 0)` keeps the C4DVector temporary as
//   retail's 16-byte frame block: VC7.1 copies it through a second zero register (`mov esi,eax`) and sinks the temporary's
//   own (dead) zero stores below the bank / index stores.
#define FABLE_RETAIL_C4DVECTOR
#define FABLE_RETAIL_C2DBOXF
#include "engine/C4DVector.h"
#include "engine/C2DBoxF.h"
class CGraphicDataBank;
class C2DAlphaMaskInfo {
public:
    C2DAlphaMaskInfo();
    void Reset();
    CGraphicDataBank* AlphaMaskBank;                 // +0x00
    unsigned long     AlphaMaskBankIndex;            // +0x04
    C4DVector         AlphaMaskPositionFactors;      // +0x08
    C2DBoxF           AlphaMaskClipBox;              // +0x18
    bool              MaskObjectsOutsideAlphaMask;   // +0x28
    bool              FilterMask;                    // +0x29
};
void C2DAlphaMaskInfo::Reset()
{
    AlphaMaskBank = 0;
    AlphaMaskBankIndex = 0;
    AlphaMaskPositionFactors = C4DVector(0.0f, 0.0f, 0.0f, 0.0f);
    MaskObjectsOutsideAlphaMask = false;
    FilterMask = false;
}