// CThingCreatureBase::GetAnimMovementVector(long animIndex) -- the animation's movement vector rotated into world space by the creature's appearance position matrix (scale 1)
// Grade: DIFFER(160v178) -- the arithmetic and matrix access match; retail additionally copies the movement X into the dead index slot and Y into a fourth frame word and reads the second and third rows' X/Y terms from those copies (a by-value parameter copy VC only partly merged); ours merges the by-value parameter with the local completely (0x3c frame)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006626E0, length 178 bytes, `ret 8` (hidden C3DVector result + index).
// Callees: CThing::PeekWorld @ 0x004C7AB0, the main mesh bank's GetAnimation(long) @ 0x00A242C0,
//   CTCGraphicAppearance::GetPositionMatrix(float) @ 0x004BC800 (CMatrix3x4 by hidden pointer; engine_api.tsv).
// Layout: retail CThing PTCGraphicAppearance +0x64; C3DAnimationInfo MovementVector +0x18; CWorld PMainMeshBank +0x3c.
// Shape: the movement vector is copied into a frame local; the rotation is the matrix's 3x3 part applied as rows
//   (x = X*m00 + Y*m10 + Z*m20, ...), built component by component in a named result returned through C3DVector's
//   memberwise copy constructor (the three `fstp` after `fxch st(2)`).
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct CMatrix3x4 {
    float m[12];
    CMatrix3x4() { }
    __forceinline C3DVector TransformVector(C3DVector v) const
    {
        C3DVector r;
        r.X = m[0] * v.X + m[3] * v.Y + m[6] * v.Z;
        r.Y = m[1] * v.X + m[4] * v.Y + m[7] * v.Z;
        r.Z = m[2] * v.X + m[5] * v.Y + m[8] * v.Z;
        return r;
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
    return PTCGraphicAppearance->GetPositionMatrix(1.0f).TransformVector(movement);
}
