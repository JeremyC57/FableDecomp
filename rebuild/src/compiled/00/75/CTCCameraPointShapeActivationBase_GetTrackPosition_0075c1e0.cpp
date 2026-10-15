// CTCCameraPointShapeActivationBase::GetTrackPosition() -- returns a copy of the parent thing's position
// Leaf: GetTrackPosition
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075C1E0, length 33 bytes. Vtable 0x0123AEDC slot 35 (shared by
// nine camera-point vtables).
// Callees: CThing::GetPosition() @ 0x004C73D0 (returns a reference to the physics position; the manifest's
// "CVertexBufferWin32::DoSizeof" label is propagated, landed callers use GetPosition).
// Identity: slot 35 is labelled "CTCObject_GetTrackPosition" in a sibling camera-point vtable; the
// manifest's "vfunc_35" is the placeholder. Layout: engine/CTCBase.h (ParentThing +4).
// Shape: the vector comes back through the hidden pointer (`ret 4`, eax = result). The copy is three
// dword moves alternating edx/eax with the result pointer loaded once into ecx: a MEMBERWISE user copy
// constructor over dword members (an implicit POD copy pulls in esi; float members would go through
// the x87), so the engine's float X/Y/Z are modelled as long here.
#include "engine/CTCBase.h"

struct C3DVector {                                  // X, Y, Z (floats in the engine; copied as dwords)
    long X;
    long Y;
    long Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

struct CThing {
    const C3DVector& GetPosition() const;           // 0x004C73D0
};

struct CTCCameraPointShapeActivationBase : CTCBase {
    C3DVector GetTrackPosition() const;
};

C3DVector CTCCameraPointShapeActivationBase::GetTrackPosition() const
{
    return ParentThing->GetPosition();
}