// CTCPhysicsStandard::vfunc_70() -- returns the second vector of the current right-handed set (RHSet+0x0c, this+0x5c) by value
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00723C00, length 24 bytes (vtable 0x01265F84 slot 70, rtti-vtables.tsv).
// Callees: none.
// Shape: `mov edx,[ecx+5Ch] ; mov eax,[esp+4] ; mov [eax],edx ...` with eax still holding the hidden pointer at `ret 4`:
// a 12-byte C3DVector returned BY VALUE through the hidden pointer. The interleaved dword-per-member copy needs C3DVector's
// memberwise user copy constructor (the implicit POD copy is a block copy through `add ecx,5Ch` + esi). Layout from Ego_r.pdb CTCPhysicsStandard (0x88 bytes): RHSet (CRightHandedSet, two C3DVectors) at +0x50, OldRHSet at
// +0x68. engine/CTCPhysicsStandard.h is a flattened POD (no virtuals, RHSet as opaque bytes), so the polymorphic class is
// modelled locally with the same offsets (as the landed constructor 0x00723FD0 does).
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };

struct CTCPhysicsStandard {
    virtual void v00();
    unsigned char   _base[0x3c];               // +0x04 CTCPhysicsBase body
    void*           PPhysicsMeshInfo;          // +0x40
    C3DVector       OffsetVector3D;            // +0x44
    CRightHandedSet RHSet;                     // +0x50
    CRightHandedSet OldRHSet;                  // +0x68
    long            RHSetLastFrameSet;         // +0x80
    C3DVector vfunc_70();
};

C3DVector CTCPhysicsStandard::vfunc_70()
{
    return RHSet.Forward;
}