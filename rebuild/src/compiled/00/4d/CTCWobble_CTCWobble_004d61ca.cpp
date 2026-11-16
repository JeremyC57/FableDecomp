// CTCWobble::CTCWobble(CThing* owner) -- thing component: CTCBase(owner), no wobble frames left,
// last rotation direction +1, zero wobble angle.
// Leaf: ??0CTCWobble@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model CTCBase (which bound ??0CTCBase@@QAE@PAVCThing@@@Z to 0x00686800) is gone;
//   the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as Ego_r.pdb and FableWin.pdb spell it (??0CTCWobble@@QAE@AAVCThing@@@Z).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D61CA, length 38 bytes.
// Callee: CTCBase::CTCBase(CThing&) @ 0x00686800 (its vtable 0x0125BE0C is CTCBase in
// rtti-vtables.tsv; the manifest's "CEngineSubPrimitive2DClockSprite" label for it is propagated).
// Vtable 0x0123DF34 = CTCWobble. PDB layout (48 bytes): CRightHandedSet ParentRHSet +0x0c (not
// initialised here), long FramesOfWobbleRemaining +0x24, float DirectionLastRotated +0x28,
// float AngleOfWobble +0x2c. The pass-through `push [esp+8]`, the `and [..],0` zero store and
// the vtable store scheduled between the two float stores are the size-optimised TU shape.
// The manifest label "CControllerDisconnect::CControllerDisconnect" is propagated and rejected.
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
class CThing;

// engine/CTCBase.h (PDB): ParentThing +0x04, ToBeRemoved +0x08, IsInGlobalUpdateTCs +0x09 -- all
// written by the base constructor @ 0x00686800 (owner stored, both flags cleared), none here.

class CTCWobble : public CTCBase { public:                   // vtable 0x0123DF34
    unsigned char _pad[0x18];                  // +0x0c..+0x24 (ParentRHSet, not initialised here)
    long  FramesOfWobbleRemaining;             // +0x24
    float DirectionLastRotated;                // +0x28
    float AngleOfWobble;                       // +0x2c
    CTCWobble(CThing& owner);
};

CTCWobble::CTCWobble(CThing& owner)
    : CTCBase(owner), FramesOfWobbleRemaining(0), DirectionLastRotated(1.0f), AngleOfWobble(0.0f)
{
}