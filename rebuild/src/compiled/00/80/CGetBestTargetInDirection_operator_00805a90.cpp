// CGetBestTargetInDirection::operator()(CTargetPoint* point) -- search functor: take the XY direction from the base position to the target point (normalised when not degenerate) and keep the point when it lies on the requested side of the test line and points more along it than the best so far
// Leaf: ??RCGetBestTargetInDirection
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00805A90, length 263 bytes, `ret 4`.
// Callees: C3DVector::AccurateNormalise @ 0x00A14440. Globals: the length tolerance @ 0x0129BA3C (1e-4f, an extern float: squared
//   from memory), float pool 0x0122DEDC = 0.0f.
// Layout = Ego_r.pdb CGetBestTargetInDirection (28 bytes): BasePos (C3DVector*) +0, TestLine +4, Direction +0x10,
//   PBestTargetPoint +0x14, CosThetaBestAngle +0x18; CTargetPoint TargetingPosition +0xc.
// Shape: the squared length and the cosine are `+=` chains in retail's term order (Y, Z, X for the dot product);
//   the XY delta is a C3DVector local (Z = 0 stored with an immediate) whose Y stays on the x87 stack; the cosine is
//   homed in the dead `point` slot; the side test is `Direction > 0 && side > 0` else `Direction <= 0 && side <= 0`,
//   each arm with its own best-cosine compare and store.
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector& AccurateNormalise();                                   // 0x00A14440 (Ego_r ?AccurateNormalise@C3DVector@@QAEAAV1@XZ)
};
extern float g_fTolerance;                              // 0x0129BA3C (1e-4f)
struct CTargetPoint {
    unsigned char _pad00[0xc];                          // PThing (CIntelligentPointer<CThing>), Index
    C3DVector TargetingPosition;                        // +0x0c
};
struct CGetBestTargetInDirection {
    const C3DVector* BasePos;                           // +0x00
    C3DVector        TestLine;                          // +0x04
    float            Direction;                         // +0x10
    CTargetPoint*    PBestTargetPoint;                  // +0x14
    float            CosThetaBestAngle;                 // +0x18
    void operator()(CTargetPoint* point);
};

void CGetBestTargetInDirection::operator()(CTargetPoint* point)
{
    C3DVector delta(point->TargetingPosition.X - BasePos->X, point->TargetingPosition.Y - BasePos->Y, 0.0f);
    float lengthSquared = delta.X * delta.X;
    lengthSquared += delta.Y * delta.Y;
    if (lengthSquared > g_fTolerance * g_fTolerance)
        delta.AccurateNormalise();
    float cosTheta = delta.Y * TestLine.Y;
    cosTheta += delta.Z * TestLine.Z;
    cosTheta += delta.X * TestLine.X;
    float side = delta.Y * TestLine.X - delta.X * TestLine.Y;
    if (Direction > 0.0f && side > 0.0f) {
        if (cosTheta > CosThetaBestAngle) {
            PBestTargetPoint = point;
            CosThetaBestAngle = cosTheta;
        }
    } else if (Direction <= 0.0f && side <= 0.0f) {
        if (cosTheta > CosThetaBestAngle) {
            PBestTargetPoint = point;
            CosThetaBestAngle = cosTheta;
        }
    }
}