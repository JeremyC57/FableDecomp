// C2DAlphaMaskInfo::C2DAlphaMaskInfo() -- construct a cleared alpha-mask info
// Leaf: ??0C2DAlphaMaskInfo@@QAE@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BE2540, 56 bytes, thiscall, plain `ret` (returns `this`).
// Identity: Ego_r.pdb / FableWin.pdb ??0C2DAlphaMaskInfo@@QAE@XZ; the class is the PDB C2DAlphaMaskInfo (0x2C bytes: AlphaMaskBank +0x00,
//   AlphaMaskBankIndex +0x04, C4DVector AlphaMaskPositionFactors +0x08, C2DBoxF AlphaMaskClipBox +0x18,
//   MaskObjectsOutsideAlphaMask +0x28, FilterMask +0x29), in the engine 2D-viewport TU (0x00BE2xxx). The catalog's
//   "CVO_OracleMarker::Init" was a propagated label. No engine header models the class yet (a local class with the PDB layout).
// Shape: the clip box is left alone; `AlphaMaskPositionFactors = C4DVector(0, 0, 0, 0)` keeps the C4DVector temporary as
//   retail's 16-byte frame block: VC7.1 copies it through a second zero register (`mov esi,eax`) and sinks the temporary's
//   own (dead) zero stores below the bank / index stores.
// The body is Reset's (0x00BE24D0) written out; `{ Reset(); }` with Reset defined above in the TU also matches, but would
//   define ?Reset@ a second time in the rebuild.
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
C2DAlphaMaskInfo::C2DAlphaMaskInfo()
{
    AlphaMaskBank = 0;
    AlphaMaskBankIndex = 0;
    AlphaMaskPositionFactors = C4DVector(0.0f, 0.0f, 0.0f, 0.0f);
    MaskObjectsOutsideAlphaMask = false;
    FilterMask = false;
}