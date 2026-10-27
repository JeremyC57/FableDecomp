// CTCPhysicsControlled::GetImpulse() const -- the ImpulseVelocity vector by value
// Leaf: ?GetImpulse@CTCPhysicsControlled@@UBE?AVC3DVector@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00725EC0, 33 bytes, thiscall, `ret 4` (the hidden result pointer).
// Identity (proven): slot 76 of vtable 0x0126616C (RTTI .?AVCTCPhysicsControlled@@; the only vtable holding it); Ego_r.pdb
//   ?GetImpulse@CTCPhysicsControlled@@UBE?AVC3DVector@@XZ is the class's only C3DVector-returning virtual and ImpulseVelocity (Ego_r.pdb +0x88) is the member
//   read at retail +0x88.
// Shape (family M138, representative 0x0056E3B0): `return ImpulseVelocity;` -- C3DVector's copy (three dword moves) into the result.
// Header debt: the class is modelled by the member's retail offset (its other members as opaque bytes).
class C3DVector {
public:
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
class CTCPhysicsControlled {
public:
    virtual ~CTCPhysicsControlled();
    virtual C3DVector GetImpulse() const;
    unsigned char _pad004[0x88 - 4];
    C3DVector ImpulseVelocity;                                        // +0x88
};
C3DVector CTCPhysicsControlled::GetImpulse() const
{
    return ImpulseVelocity;
}