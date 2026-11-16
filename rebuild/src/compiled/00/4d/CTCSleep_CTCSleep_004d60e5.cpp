// CTCSleep::CTCSleep(CThing&) -- base CTCBase(thing), own vtable, IsReadyToSleep/FastTrack false, LastFrameWokeUp = -1000000
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Leaf: ??0CTCSleep@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model CTCBase_Calls (which bound ??0CTCBase_Calls@@QAE@PAVCThing@@@Z to 0x00686800) is gone;
//   the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as the PDBs spell it (??0CTCSleep@@QAE@AAVCThing@@@Z).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D60E5, length 39 bytes.
// Callees: CTCBase::CTCBase(CThing&) @ 0x00686800 (stores the parent thing at +4, clears the two flags
// at +8/+9, vtable 0x0125BE0C = CTCBase; the manifest's "CEngineSubPrimitive2DClockSprite" label is
// propagated). Vtable 0x0123DD94 = CTCSleep (rtti-vtables.tsv).
// PDB CTCSleep (0x20 bytes): IsReadyToSleep +0xc, FastTrack +0xd, LastFrameWokeUp +0x10,
// PosAtFastTrackSetting (C3DVector) +0x14 -- the vector is left uninitialised. 0xFFF0BDC0 is
// -1000000 ("woke up a long time ago"). The engine header for CTCBase is a flat POD, hence `_Calls`. [superseded 2026-11-16: the header's FABLE_RETAIL_CTCBASE branch is polymorphic; see the Ported note]
// The pass-through `push dword ptr [esp+8]` and immediate byte stores mark a size-optimised TU.
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
class CThing;

#include "engine/C3DVector.h"

class CTCSleep : public CTCBase { public:
    bool      IsReadyToSleep;                               // +0x0c
    bool      FastTrack;                                    // +0x0d
    long      LastFrameWokeUp;                              // +0x10
    C3DVector PosAtFastTrackSetting;                        // +0x14 (not initialised)
    CTCSleep(CThing& parent);
};

CTCSleep::CTCSleep(CThing& parent)
    : CTCBase(parent)
    , IsReadyToSleep(false)
    , FastTrack(false)
    , LastFrameWokeUp(-1000000)
{
}