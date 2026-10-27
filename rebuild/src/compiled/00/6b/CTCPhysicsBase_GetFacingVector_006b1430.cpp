// CTCPhysicsBase::GetFacingVector(float) const -- returns the constant vector (0.0f, 1.0f, 0.0f)
// Leaf: ?GetFacingVector@CTCPhysicsBase@@UBE?AVC3DVector@@M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B1430, 27 bytes, thiscall, hidden return pointer, `ret 8`.
// Identity (proven): slot 69 of vtable 0x0125D2C4 (RTTI .?AVCTCPhysicsBase@@; also CTCPhysicsLight slot 69 and a third vtable 0x012699E0 slot 40); one float argument (`ret 8`), returns (0, 1, 0); Ego_r.pdb ?GetFacingVector@CTCPhysicsBase@@UBE?AVC3DVector@@M@Z, adjacent to the no-argument overload at slot 70.
// Shape (family M018, representative 0x00415CD0): `return C3DVector(x, y, z);` -- the constructor stores the three
//   immediates through the hidden return pointer (no NRVO question: nothing is named); `this` is unused.
// Header debt: engine/CTCPhysicsBase.h is a generated flat layout without the virtual interface, so the class is declared here
//   with the one virtual this TU defines.
class C3DVector {                                         // Ego_r.pdb: X, Y, Z (12 bytes)
public:
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    float X;
    float Y;
    float Z;
};
class CTCPhysicsBase {
public:
    virtual C3DVector GetFacingVector(float arg) const;
};
C3DVector CTCPhysicsBase::GetFacingVector(float arg) const
{
    return C3DVector(0.0f, 1.0f, 0.0f);
}