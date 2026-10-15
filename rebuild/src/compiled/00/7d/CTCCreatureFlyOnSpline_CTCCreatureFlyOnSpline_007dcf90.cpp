// CTCCreatureFlyOnSpline::CTCCreatureFlyOnSpline(CThing*) -- CTCBase(thing), own vtable, no spline, no engine handles, progress 0, delta progress 0.01, not looping
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007DCF90, length 55 bytes (+ int3 padding).
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800 (the manifest label "CEngineSubPrimitive2DClockSprite"
// is propagated). Vtable 0x0126F9F4 = CTCCreatureFlyOnSpline.
// Layout = Ego_r.pdb CTCCreatureFlyOnSpline less 4 bytes from +0x20 (the retail std::vector is 12
// bytes): PSpline +0xc (CCountedPointer<CCardinalSpline>: object + count block), EngineHandles +0x14
// (vector<CEnginePrimitiveHandle>), Progress +0x20, DeltaProgress +0x24, Looping +0x28.
// Shape: every store follows the vtable store (the counted pointer and the vector are member objects
// with inline constructors); the zeroes share eax, the 0.01f delta is an immediate.
struct CThing;
struct CCardinalSpline;
struct CEnginePrimitiveHandle;

struct CTCBase_Calls {                                      // engine/CTCBase.h layout (0xc bytes)
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    CTCBase_Calls(CThing* thing);                           // 0x00686800
};

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

struct CTCCreatureFlyOnSpline : CTCBase_Calls {             // vtable 0x0126F9F4
    CCountedPointer_CardinalSpline PSpline;                 // +0x0c
    CRetailVector_EngineHandle     EngineHandles;           // +0x14
    float                          Progress;                // +0x20
    float                          DeltaProgress;           // +0x24
    bool                           Looping;                 // +0x28
    virtual void v00();
    CTCCreatureFlyOnSpline(CThing* thing);
};

CTCCreatureFlyOnSpline::CTCCreatureFlyOnSpline(CThing* thing)
    : CTCBase_Calls(thing)
    , PSpline()
    , EngineHandles()
    , Progress(0.0f)
    , DeltaProgress(0.01f)
    , Looping(false)
{
}