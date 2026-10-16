// CVillageTaskSecuritySweep::CVillageTaskSecuritySweep(const C3DVector& position, float radius, const C3DVector& lookTowards, float angle) -- village task ctor: base task, own vtable, an empty CreatureAssignedToTask intelligent pointer, the stand position/radius and look target/angle copied, HasBecomeInvalid cleared
// Grade: DIFFER(98v98) -- register scheduling only: retail copies each argument word through eax/ecx in strict load/store order, ours interleaves the next argument load through edx (`mov edx,[eax+4] ; mov ecx,[esp+10h]`); the implicit POD copy additionally formed `lea` bases (fixed by a memberwise copy ctor or operator=); tried init list, body assignments, user operator=, user copy ctor
// Leaf: ??0CVillageTaskSecuritySweep
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065DCE8, length 98 bytes, `ret 10h`.
// Callees: CVillageTaskBase::CVillageTaskBase @ 0x0099A380 (catalog base ctor), CBaseIntelligentPointer::CBaseIntelligentPointer
// @ 0x00A01B10 (then the inline CIntelligentPointer<const CThing> vtable 0x01237F1C).
// Vtable 0x0125A868 (rtti-vtables.tsv: CVillageTaskSecuritySweep). Layout: engine/CVillageTaskSecuritySweep.h
// (CreatureAssignedToTask +0x08, PositionToGoTo +0x10, RadiusAroundThePositionToStay +0x1c, PositionToLookTowards +0x20,
// AngleToLookTowards +0x2c, HasBecomeInvalid +0x30); the header flattens the vectors, so a local model with its names.
// Shape: the vectors are copy-constructed in the init list through C3DVector's memberwise user copy ctor (the implicit block
// copy forms a `lea` base); the float words move through integer registers.
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
struct CBaseIntelligentPointer {
    virtual void v00();
    void* PItem;
    CBaseIntelligentPointer();                     // 0x00A01B10
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01237F1C
    CIntelligentPointer() {}
    virtual void v00();
};
struct CThing;
struct CVillageTaskBase {                          // 8 bytes: vptr + one word
    virtual void v00();
    long m_word04;                                 // +0x04
    CVillageTaskBase();                            // 0x0099A380
};
struct CVillageTaskSecuritySweep : CVillageTaskBase {   // vtable 0x0125A868
    CIntelligentPointer<const CThing> CreatureAssignedToTask;   // +0x08
    C3DVector PositionToGoTo;                      // +0x10
    float     RadiusAroundThePositionToStay;       // +0x1c
    C3DVector PositionToLookTowards;               // +0x20
    float     AngleToLookTowards;                  // +0x2c
    bool      HasBecomeInvalid;                    // +0x30
    virtual void v00();
    CVillageTaskSecuritySweep(const C3DVector& position, float radius, const C3DVector& lookTowards, float angle);
};
CVillageTaskSecuritySweep::CVillageTaskSecuritySweep(const C3DVector& position, float radius, const C3DVector& lookTowards, float angle)
    : CVillageTaskBase()
    , CreatureAssignedToTask()
    , PositionToGoTo(position)
    , RadiusAroundThePositionToStay(radius)
    , PositionToLookTowards(lookTowards)
    , AngleToLookTowards(angle)
    , HasBecomeInvalid(false)
{
}
