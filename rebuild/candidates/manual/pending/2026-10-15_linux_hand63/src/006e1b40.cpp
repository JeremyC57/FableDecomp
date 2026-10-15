// CParticleEmitter::GetCurrentOrientation() const -- returns the emitter's orientation as a CRightHandedSet: Up = row 2 and Forward = row 1 of the current Transform
// Leaf: GetCurrentOrientation
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E1B40, length 78 bytes (`ret 4`: hidden result pointer).
// Callees: none (everything inline).
// Layout: Transform (CMatrix3x4: rows X, Y, Z, position) at +0xb0 in the retail emitter -- the PDB
// +0xbc less the 0xc the retail 12-byte vector and 16-byte CVectorMaps save; CParticleEmitter::
// SetOrientation (0x00AEDE60) writes the cross product into +0xb0..+0xb8 and the orientation's
// Forward into +0xbc.., so the rows read here (+0xbc.., +0xc8..) are Y (Forward) and Z (Up).
// engine/CParticleEmitter.h carries the PDB offset and an opaque matrix, so the class is modelled
// locally. PDB CRightHandedSet = { C3DVector Up; C3DVector Forward; }.
// Shape: CRightHandedSet(const C3DVector& up, const C3DVector& forward) built from two by-value row
// accessors (the matrix returns rows by value, arguments evaluated right to left): row 1 lands in
// the one 12-byte frame temporary (its Z goes through the frame as an integer, X/Y stay on the x87),
// row 2 is loaded straight onto the x87, and the memberwise C3DVector copy constructors store all
// six floats into the result (`fxch st(2)` + `fstp` chain).

struct C3DVector {
    float X;
    float Y;
    float Z;
    C3DVector() {}
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) {}
};

struct CRightHandedSet {
    C3DVector Up;                                                         // +0x00
    C3DVector Forward;                                                    // +0x0c
    CRightHandedSet(const C3DVector& up, const C3DVector& forward) : Up(up), Forward(forward) {}
};

struct CMatrix3x4 {
    C3DVector Row[4];
    C3DVector GetRow(long index) const { return Row[index]; }
};

struct CParticleEmitter {
    virtual void v00();
    unsigned char _state[0xac];                                           // +0x04 .. +0xb0
    CMatrix3x4    Transform;                                              // +0xb0
    CRightHandedSet GetCurrentOrientation() const;
};

CRightHandedSet CParticleEmitter::GetCurrentOrientation() const
{
    return CRightHandedSet(Transform.GetRow(2), Transform.GetRow(1));
}
