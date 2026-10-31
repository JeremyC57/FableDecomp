// Grade: DIFFER(799v773) 74 diff lines (parked 85): the limit test spelled as the explicit x*x+y*y+z*z sum gives retail's z,y,x load order; left: VC7.1 reassociates every (m*m)/l2 term to (m/l2)*m (source order inert), the ly2/lz2 frame slots swap, and ThisSpline's data pointer loads before InitialSpline's
// NParticleEngine::CPSCSplineBase::CSplineRandomOffsetData::OffsetSplinePositions() -- random-walk each control point of the spline away from the initial spline: step the offset by a random amount within +-speed per axis, then pull it back inside the limit ellipsoid (an axis with no room is pinned to zero)
// Leaf: OffsetSplinePositions
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B206B0, 773 bytes, plain `ret`.
// Callees: GFFloatRandom(float range, unsigned long* seed) 0x004978A0 (__fastcall, seed in ecx) on the seed at 0x013999C4.
//   Pool: 1e-4f 0x0129BA3C (extern, squared at run time), 1e-4f 0x0129F0BC, 0.0f 0x0122DEDC, 1.0f 0x0122DED8.
// Layout: as the landed ctor 0x00B21DD0 / ReadBinary 0x00B22100: InitialSpline +0, ThisSpline +4 (CArray<C3DVector> first),
//   PreviousSpline +0x18, flags +0x2C/+0x2D, SplinePosOffsetLimits +0x30, SplinePosOffsetSpeed +0x3C (12-byte STLport vectors).
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    void operator+=(const C3DVector& o) { X += o.X; Y += o.Y; Z += o.Z; }
    float GetLengthSquared() const
    {
        float d = X * X;
        d += Y * Y;
        d += Z * Z;
        return d;
    }
};
inline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
inline C3DVector operator+(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X + b.X, a.Y + b.Y, a.Z + b.Z); }
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0
extern unsigned long g_SplineRandomSeed;                            // 0x013999C4
extern const float g_epsilon_0129ba3c;                              // 1e-4f
template <class T> struct CArray {
    T* m_begin; T* m_end; T* m_capacity;
    long Size() const { return (long)(m_end - m_begin); }
    T& operator[](long n) { return m_begin[n]; }
};
struct CCardinalSpline {
    CArray<C3DVector> ControlPoints;
    float             Tension;
    bool              EndPointsConnected;
};
namespace NParticleEngine {
struct CPSCSplineBase {
    struct CSplineRandomOffsetData {
        CCardinalSpline*  InitialSpline;
        CCardinalSpline   ThisSpline;
        CCardinalSpline   PreviousSpline;
        bool              OffsetOnlyAtStartup;
        bool              ResetEachFrame;
        CArray<C3DVector> SplinePosOffsetLimits;
        CArray<C3DVector> SplinePosOffsetSpeed;
        void OffsetSplinePositions();
    };
};
void CPSCSplineBase::CSplineRandomOffsetData::OffsetSplinePositions()
{
    long count = ThisSpline.ControlPoints.Size();
    for (long i = 0; i < count; ++i) {
        const C3DVector& limit = SplinePosOffsetLimits[i];
        if (limit.X * limit.X + limit.Y * limit.Y + limit.Z * limit.Z > g_epsilon_0129ba3c * g_epsilon_0129ba3c) {
            const C3DVector& initialPoint = InitialSpline->ControlPoints[i];
            C3DVector offset = ThisSpline.ControlPoints[i] - initialPoint;
            const C3DVector& speed = SplinePosOffsetSpeed[i];
            C3DVector change;
            change.X = speed.X != 0.0f ? GFFloatRandom(speed.X * 2.0f, &g_SplineRandomSeed) - speed.X : 0.0f;
            change.Y = speed.Y != 0.0f ? GFFloatRandom(speed.Y * 2.0f, &g_SplineRandomSeed) - speed.Y : 0.0f;
            change.Z = speed.Z != 0.0f ? GFFloatRandom(speed.Z * 2.0f, &g_SplineRandomSeed) - speed.Z : 0.0f;
            change += offset;
            C3DVector& moved = change;
            float limitX2 = limit.X * limit.X;
            float limitY2 = limit.Y * limit.Y;
            float limitZ2 = limit.Z * limit.Z;
            if (limitX2 < 0.0001f) {
                moved.X = 0.0f;
                float d = moved.Y * moved.Y / limitY2 + moved.Z * moved.Z / limitZ2;
                if (d > 1.0f) {
                    float scale = 1.0f / d;
                    moved.Y *= scale;
                    moved.Z *= scale;
                }
            } else if (limitY2 < 0.0001f) {
                moved.Y = 0.0f;
                float d = moved.X * moved.X / limitX2 + moved.Z * moved.Z / limitZ2;
                if (d > 1.0f) {
                    float scale = 1.0f / d;
                    moved.X *= scale;
                    moved.Z *= scale;
                }
            } else if (limitZ2 < 0.0001f) {
                moved.Z = 0.0f;
                float d = moved.X * moved.X / limitX2 + moved.Y * moved.Y / limitY2;
                if (d > 1.0f) {
                    float scale = 1.0f / d;
                    moved.X *= scale;
                    moved.Y *= scale;
                }
            } else {
                float d = moved.X * moved.X / limitX2 + moved.Y * moved.Y / limitY2 + moved.Z * moved.Z / limitZ2;
                if (d > 1.0f) {
                    float scale = 1.0f / d;
                    moved.X *= scale;
                    moved.Y *= scale;
                    moved.Z *= scale;
                }
            }
            ThisSpline.ControlPoints[i] = moved + initialPoint;
        }
    }
}
}
