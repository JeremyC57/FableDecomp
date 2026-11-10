#ifndef FABLE_ENGINE_CTCANIMATIONCOMPLEX_H
#define FABLE_ENGINE_CTCANIMATIONCOMPLEX_H
// CTCAnimationComplex (new shared header, chunk 1006, row 0x005B3E4E CTCInventoryClothing::GlobalUpdate): the animation component
// (RTTI .?AVCTCAnimationComplex@@ : CTCAnimationBase : CTCBase; Ego_r.pdb sizeof 0x10, PAnimation +0x0C; members not modelled).
// StatGetInterfaceType: TCI_ANIMATION (90; the `push 5Ah ; pop edi` of the caller's inline GetTC, 0x005B3F95). Out of line:
// ?GetAnimation@CTCAnimationComplex@@QAEAAVCAnimateThing@@XZ (Ego_r.pdb / FableWin.pdb, retail 0x0070B460). Declarations only.
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
class CAnimateThing;
class CTCAnimationComplex : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_ANIMATION; }
    CAnimateThing& GetAnimation();                           // 0x0070B460
};
#endif // FABLE_ENGINE_CTCANIMATIONCOMPLEX_H
