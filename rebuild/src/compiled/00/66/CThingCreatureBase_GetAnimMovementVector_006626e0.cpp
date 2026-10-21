// CThingCreatureBase::GetAnimMovementVector(long animIndex) -- the animation's movement vector rotated into world space by the creature's appearance position matrix (scale 1)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006626E0, length 178 bytes, `ret 8` (hidden C3DVector result + index).
// Callees: CThing::PeekWorld @ 0x004C7AB0, the main mesh bank's GetAnimation(long) @ 0x00A242C0,
//   CTCGraphicAppearance::GetPositionMatrix(float) @ 0x004BC800 (CMatrix3x4 by hidden pointer; engine_api.tsv).
// Layout: retail CThing PTCGraphicAppearance +0x64; C3DAnimationInfo MovementVector +0x18; CWorld PMainMeshBank +0x3c.
// Shape: the movement vector is copied into a frame local and rotated IN PLACE by an inline matrix helper that first copies
//   the components into float locals (x = X*m00 + Y*m10 + Z*m20, ...; the helper's name is not from the PDB): VC7.1 keeps
//   the copies of X (dead index slot) and Y (a fourth frame word) because the first stores overwrite them, Z is read from
//   the local; the local is then returned through C3DVector's memberwise copy constructor (`fxch st(2)` + three `fstp`).
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct CMatrix3x4 {
    float m[12];
    CMatrix3x4() { }
    void RotateInPlace(C3DVector& v) const
    {
        float x = v.X;
        float y = v.Y;
        float z = v.Z;
        v.X = m[0] * x + m[3] * y + m[6] * z;
        v.Y = m[1] * x + m[4] * y + m[7] * z;
        v.Z = m[2] * x + m[5] * y + m[8] * z;
    }
};
struct C3DAnimationInfo {
    unsigned char _pad[0x18];
    C3DVector MovementVector;                  // +0x18
};
struct CMeshDataBank {
    C3DAnimationInfo* GetAnimation(long index);   // 0x00A242C0
};
struct CWorld {
    unsigned char _pad[0x3c];
    CMeshDataBank* PMainMeshBank;              // +0x3c
};
struct CTCGraphicAppearance {
    CMatrix3x4 GetPositionMatrix(float scale);  // 0x004BC800
};
struct CThing {
    unsigned char _pad[0x64];
    CTCGraphicAppearance* PTCGraphicAppearance; // +0x64
    const CWorld& PeekWorld() const;           // 0x004C7AB0
};
struct CThingCreatureBase : CThing {
    C3DVector GetAnimMovementVector(long animIndex);
};

C3DVector CThingCreatureBase::GetAnimMovementVector(long animIndex)
{
    CMeshDataBank* bank = PeekWorld().PMainMeshBank;
    C3DVector movement = bank->GetAnimation(animIndex)->MovementVector;
    PTCGraphicAppearance->GetPositionMatrix(1.0f).RotateInPlace(movement);
    return movement;
}