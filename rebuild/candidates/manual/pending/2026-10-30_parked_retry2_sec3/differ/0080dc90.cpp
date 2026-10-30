// CThingFilter_IsEvadeExplosion::operator()(CThing* thing) const -- a rolling thing passes when its flattened travel direction and the flattened direction from the explosion line (StartPos..EndPos) to it satisfy the filter's direction test
// Grade: DIFFER(242v242) -- IMPROVED (21->17 diff lines): the physics position as a named const C3DVector& (retail's lea ecx,[eax+0Ch] kept for the GetLineOut argument); left: x87 spill choice (retail keeps travel X, spills Y). Tried: ctor form with the reference (Y first, 243), operator- + z=0 (243), three GetLength spellings
// Leaf: ??RCThingFilter_IsEvadeExplosion
// Retail: Fable.exe (TLC, VC7.1) @ 0x0080DC90, 240 bytes (+16 int3), `ret 4`, bool in al.
// Callees: IsRolling(CThing*) @ 0x0085EEB0 (`__fastcall`), GetLineOutFromClosestPointOnFiniteLineToPoint(const C3DVector&,
//   const C3DVector&, const C3DVector&) @ 0x008F5B50 (`__fastcall`, hidden result in ecx), C3DVector::Normalise @ 0x00A14440,
//   the filter's direction test @ 0x0080C140 (manifest "CCompareTargetedPositions::operator()" is propagated; name not
//   recovered). Globals: 1e-4f 0x0129BA3C (an extern const, squared at run time), 1.0f 0x0122DED8, 0.0f 0x0122DEDC.
// Layout: PDB CThingFilter_IsEvadeExplosion { C3DVector* StartPos; C3DVector* EndPos; }; the thing's physics component
//   (+0x60) keeps its position at +0xC and the previous frame's at +0x18.
// Shape: the travel vector is (dx, dy, 0) in a frame local; its length (sqrt intrinsic) guards a `1 / len` scale (the z
//   term folds to `inv * 0`); the line-out vector is flattened (z = 0) and normalised only when longer than epsilon.
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
extern const float g_fEpsilon;                  // 0x0129BA3C (1e-4f)
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    float GetLength() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
    void Normalise();                           // 0x00A14440
};
struct CTCPhysicsBase {
    unsigned char _pad00[0x0c];
    C3DVector Position;                         // +0x0c
    C3DVector PreviousPosition;                 // +0x18
};
struct CThing {
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
};
bool __fastcall IsRolling(CThing* thing);       // 0x0085EEB0
C3DVector __fastcall GetLineOutFromClosestPointOnFiniteLineToPoint(const C3DVector& start, const C3DVector& end,
                                                                  const C3DVector& point);   // 0x008F5B50
struct CThingFilter_IsEvadeExplosion {
    C3DVector* StartPos;                        // +0x00
    C3DVector* EndPos;                          // +0x04
    bool IsEvading(const C3DVector& lineOut, const C3DVector& travel) const;   // 0x0080C140
    bool operator()(CThing* thing) const;
};

bool CThingFilter_IsEvadeExplosion::operator()(CThing* thing) const
{
    if (IsRolling(thing)) {
        CTCPhysicsBase* physics = thing->PhysicsTC;
        const C3DVector& pos = physics->Position;
        C3DVector travel;
        travel.X = pos.X - physics->PreviousPosition.X;
        travel.Y = pos.Y - physics->PreviousPosition.Y;
        travel.Z = 0.0f;
        float length = travel.GetLength();
        if (length > g_fEpsilon) {
            float scale = 1.0f / length;
            travel.X *= scale;
            travel.Y *= scale;
            travel.Z *= scale;
            C3DVector lineOut = GetLineOutFromClosestPointOnFiniteLineToPoint(*StartPos, *EndPos, pos);
            lineOut.Z = 0.0f;
            if (lineOut.GetLengthSquared() > g_fEpsilon * g_fEpsilon) {
                lineOut.Normalise();
                if (IsEvading(lineOut, travel))
                    return true;
            }
        }
    }
    return false;
}
