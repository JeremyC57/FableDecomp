// CGetBestTargetInDirection::CGetBestTargetInDirection(const C3DVector* basePos, const C3DVector& towards, float direction) -- the search functor for the best target along a direction: the flattened (Z = 0) unit line from the base position towards the point, no best target yet, best cosine -2
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008059F0, length 140 bytes (padded to 160).
// Callees: C3DVector::Normalise @ 0x00A14440 (manifest "C3DVector::GetScaled", propagated; it scales in place and returns
//          *this). Global: 0.0001f @ 0x0129BA3C (squared for the length test).
// Shape: init list BasePos / Direction / PBestTargetPoint / CosThetaBestAngle (the shared `xor edx,edx` zero); TestLine is
// assigned in the body from `towards - *basePos` (a hidden-pointer temporary copied through the frame), its Z cleared
// with the same zero register, then normalised when its squared length beats the squared epsilon.
// Layout: Ego_r.pdb (BasePos +0, TestLine +4, Direction +0x10, PBestTargetPoint +0x14, CosThetaBestAngle +0x18).
// Leaf: ??0CGetBestTargetInDirection
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
    C3DVector& Normalise();                        // 0x00A14440
};
extern const float g_minLength_0129ba3c;           // 0x0129BA3C (0.0001f)
struct CTargetPoint;
struct CGetBestTargetInDirection {
    const C3DVector* BasePos;                      // +0x00
    C3DVector        TestLine;                     // +0x04
    float            Direction;                    // +0x10
    CTargetPoint*    PBestTargetPoint;             // +0x14
    float            CosThetaBestAngle;            // +0x18
    CGetBestTargetInDirection(const C3DVector* basePos, const C3DVector& towards, float direction);
};

CGetBestTargetInDirection::CGetBestTargetInDirection(const C3DVector* basePos, const C3DVector& towards, float direction)
    : BasePos(basePos), Direction(direction), PBestTargetPoint(0), CosThetaBestAngle(-2.0f)
{
    TestLine = towards - *basePos;
    TestLine.Z = 0.0f;
    if (TestLine.GetLengthSquared() > g_minLength_0129ba3c * g_minLength_0129ba3c)
        TestLine.Normalise();
}