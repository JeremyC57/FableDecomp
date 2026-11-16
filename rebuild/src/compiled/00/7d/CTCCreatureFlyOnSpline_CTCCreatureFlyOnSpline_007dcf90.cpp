// CTCCreatureFlyOnSpline::CTCCreatureFlyOnSpline(CThing&) -- CTCBase(thing), own vtable, no spline, no engine handles, progress 0, delta progress 0.01, not looping
// Leaf: ??0CTCCreatureFlyOnSpline@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model CTCBase_Calls (which bound ??0CTCBase_Calls@@QAE@PAVCThing@@@Z to 0x00686800) is gone;
//   the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as the PDBs spell it (??0CTCCreatureFlyOnSpline@@QAE@AAVCThing@@@Z).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007DCF90, length 55 bytes (+ int3 padding).
// Callees: CTCBase::CTCBase(CThing&) @ 0x00686800 (the manifest label "CEngineSubPrimitive2DClockSprite"
// is propagated). Vtable 0x0126F9F4 = CTCCreatureFlyOnSpline.
// Layout = Ego_r.pdb CTCCreatureFlyOnSpline less 4 bytes from +0x20 (the retail std::vector is 12
// bytes): PSpline +0xc (CCountedPointer<CCardinalSpline>: object + count block), EngineHandles +0x14
// (vector<CEnginePrimitiveHandle>), Progress +0x20, DeltaProgress +0x24, Looping +0x28.
// Shape: every store follows the vtable store (the counted pointer and the vector are member objects
// with inline constructors); the zeroes share eax, the 0.01f delta is an immediate.
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
class CThing;
class CCardinalSpline;
class CEnginePrimitiveHandle;

struct CCountedPointer_CardinalSpline {                     // CCountedPointer<CCardinalSpline>: 8 bytes
    CCardinalSpline* Object;
    void*            CountBlock;
    CCountedPointer_CardinalSpline() : Object(0), CountBlock(0) { }
};

struct CRetailVector_EngineHandle {                         // retail std::vector<CEnginePrimitiveHandle>: 12 bytes
    CEnginePrimitiveHandle* First;
    CEnginePrimitiveHandle* Last;
    CEnginePrimitiveHandle* End;
    CRetailVector_EngineHandle() : First(0), Last(0), End(0) { }
};

class CTCCreatureFlyOnSpline : public CTCBase { public:             // vtable 0x0126F9F4
    CCountedPointer_CardinalSpline PSpline;                 // +0x0c
    CRetailVector_EngineHandle     EngineHandles;           // +0x14
    float                          Progress;                // +0x20
    float                          DeltaProgress;           // +0x24
    bool                           Looping;                 // +0x28
    CTCCreatureFlyOnSpline(CThing& thing);
};

CTCCreatureFlyOnSpline::CTCCreatureFlyOnSpline(CThing& thing)
    : CTCBase(thing)
    , PSpline()
    , EngineHandles()
    , Progress(0.0f)
    , DeltaProgress(0.01f)
    , Looping(false)
{
}