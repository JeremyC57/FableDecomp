// CTCParamsPhysicsBase::CTCParamsPhysicsBase(const C3DVector&, const CRightHandedSet&) -- base, vtable, copy Position and Rhs
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A06E0, length 80 bytes.
// Callees: the root base constructor CBase::CBase() @ 0x0099A2F0 (no arguments).
// PDB CTCParamsPhysicsBase (0x28 bytes): Position (C3DVector) +0x04, Rhs (CRightHandedSet, two C3DVectors) +0x10.
// Shape: both members copied word by word through ecx/edx/eax, each argument reference loaded from its slot right before
// its copy: C3DVector has a MEMBERWISE user copy constructor (the implicit copy of a plain float record copies through
// `lea eax,[esi+4]` instead; a plain float copy needs no x87 here).
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };

struct CBase_006a06e0 {
    virtual void v00();
    CBase_006a06e0();                               // 0x0099A2F0
};

struct CTCParamsPhysicsBase : CBase_006a06e0 {
    C3DVector       Position;                       // +0x04
    CRightHandedSet Rhs;                            // +0x10
    CTCParamsPhysicsBase(const C3DVector& position, const CRightHandedSet& rhs);
    virtual void v00();
};

CTCParamsPhysicsBase::CTCParamsPhysicsBase(const C3DVector& position, const CRightHandedSet& rhs)
    : Position(position), Rhs(rhs)
{
}
