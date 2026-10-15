// CParticleEmitter::SetPosition(const C3DPositionInterpolator&) -- unless the emitter is 2D, take the interpolator's start/end positions and write them into the translation rows of Transform and EndTransform
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AEDDC0, length 150 bytes (padded to 160).
// Leaf: SetPosition
// Callees: none.
// Layout (retail): bit-field word +0x40 (Emitter2D = bit 11), PositionInterpolator +0x94 (C3DPositionInterpolator: vptr,
// StartPosition +4, EndPosition +0x10), Transform +0xb0, EndTransform +0x110 (CMatrix3x4, translation at +0x24) -- the PDB
// offsets minus the 12 bytes of narrower retail containers.
// Shape: the flag through an inline `bool IsEmitter2D()` accessor (whole-word `xor eax,eax ; mov ax ; shr eax,0Bh ; test al,1`;
// the raw bit-field gives `test byte ptr [ecx+41h],8`); the interpolator copied by its IMPLICIT operator= (vptr kept, both
// C3DVectors through lea'd pointers), then the matrix translations set by an inline three-float setter that re-reads the
// members straight off ecx (assigning the two vectors one by one keeps the lea'd pointers alive instead).
struct C3DVector {
    float X, Y, Z;
};
struct C3DPositionInterpolator {
    virtual void v00();
    C3DVector StartPosition;                // +0x04
    C3DVector EndPosition;                  // +0x10
};
struct CMatrix3x4 {
    C3DVector Right;                        // +0x00
    C3DVector Up;                           // +0x0c
    C3DVector Forward;                      // +0x18
    float     PositionX;                    // +0x24
    float     PositionY;                    // +0x28
    float     PositionZ;                    // +0x2c
    void SetPosition(const C3DVector& v) { PositionX = v.X; PositionY = v.Y; PositionZ = v.Z; }
};
struct CParticleEmitter {
    virtual void v00();
    unsigned char  _base4[0x3c];            // +0x04 .. +0x3f
    unsigned short Priority                 : 3;   // +0x40 bits 0..2
    unsigned short IsScreenDisplacement     : 1;
    unsigned short ReadZBuffer              : 1;
    unsigned short Dead                     : 1;
    unsigned short ParticleCreationDisabled : 1;
    unsigned short ContinuousEmitter        : 1;
    unsigned short Initialised              : 1;
    unsigned short WaitingToSleep           : 1;
    unsigned short Sleeping                 : 1;
    unsigned short Emitter2D                : 1;   // bit 11
    unsigned char  _body42[0x94 - 0x42];
    C3DPositionInterpolator PositionInterpolator;  // +0x94
    CMatrix3x4     Transform;               // +0xb0
    CMatrix3x4     InverseTransform;        // +0xe0
    CMatrix3x4     EndTransform;            // +0x110
    bool IsEmitter2D() const { return Emitter2D; }
    void SetPosition(const C3DPositionInterpolator& position);
};
void CParticleEmitter::SetPosition(const C3DPositionInterpolator& position)
{
    if (!IsEmitter2D()) {
        PositionInterpolator = position;
        Transform.SetPosition(PositionInterpolator.StartPosition);
        EndTransform.SetPosition(PositionInterpolator.EndPosition);
    }
}
