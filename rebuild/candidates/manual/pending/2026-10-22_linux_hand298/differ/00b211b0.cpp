// Grade: DIFFER(633v634) -- ebx/ebp colouring swap only: retail keeps other in ebx and &ThisSpline.ControlPoints (then t) in ebp; ours the reverse (the byte is the [ebp+0] disp8)
// NParticleEngine::CPSCSplineBase::CSplineRandomOffsetData::InterpolateTo(const CSplineRandomOffsetData& other, float t) -- blend the random-offset state towards another: lerp both splines' tension and points, re-snapshot the previous spline if its point count no longer matches, take the other's flags past t = 0.5 and interpolate the offset limit/speed arrays
// Leaf: InterpolateTo@CSplineRandomOffsetData
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B211B0, 634 bytes, `ret 8`. Called from the landed CPSCSplineBase::InterpolateTo
//   0x00B22310.
// Callees: CArray<C3DVector> copy ctor 0x006F0810 / operator= 0x0075B8D0, GFInterpolateArray<C3DVector> 0x00B15BD0
//   (__fastcall, hidden result in ecx, first array in edx), free 0x00BFEA14 (inline ~CArray). Floats 1.0 0x0122DED8,
//   0.5 0x0122F59C.
// Layout: as the landed ctor 0x00B21DD0: ThisSpline +4, PreviousSpline +0x18, flags +0x2C/+0x2D, SplinePosOffsetLimits
//   +0x30, SplinePosOffsetSpeed +0x3C.
// Shape: the CPSCSplineBase::InterpolateTo model applied per spline (by-value GetControlPoints copies, zero default
//   vector); `1 - t` lives in a named float; the offset arrays are passed by reference (no copies).
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
template <class T> struct CArray;
template <class T> CArray<T> __fastcall GFInterpolateArray(const CArray<T>& a, const CArray<T>& b, float t, const T& defaultValue);
template <class T> struct CArray {                     // STLport vector<T> (12 bytes in retail)
    T* m_begin; T* m_end; T* m_capacity;
    CArray(const CArray& other);                       // 0x006F0810 (T = C3DVector)
    ~CArray() { if (m_begin) free(m_begin); }
    long Size() const { return (long)(m_end - m_begin); }
    CArray& operator=(const CArray& other);            // 0x0075B8D0 (T = C3DVector)
};
struct CCardinalSpline {
    CArray<C3DVector> ControlPoints;                   // +0x00
    CArray<C3DVector> GetControlPoints() const { return ControlPoints; }
    float             Tension;                         // +0x0c
    bool              EndPointsConnected;              // +0x10
};

namespace NParticleEngine {
struct CPSCSplineBase {
    struct CSplineRandomOffsetData {
        CCardinalSpline*  InitialSpline;          // +0x00
        CCardinalSpline   ThisSpline;             // +0x04
        CCardinalSpline   PreviousSpline;         // +0x18
        bool              OffsetOnlyAtStartup;    // +0x2C
        bool              ResetEachFrame;         // +0x2D
        CArray<C3DVector> SplinePosOffsetLimits;  // +0x30
        CArray<C3DVector> SplinePosOffsetSpeed;   // +0x3C
        void InterpolateTo(const CSplineRandomOffsetData& other, float t);
    };
};

void CPSCSplineBase::CSplineRandomOffsetData::InterpolateTo(const CSplineRandomOffsetData& other, float t)
{
    float s = 1.0f - t;
    ThisSpline.Tension = s * ThisSpline.Tension + t * other.ThisSpline.Tension;
    ThisSpline.ControlPoints = GFInterpolateArray(ThisSpline.GetControlPoints(), other.ThisSpline.GetControlPoints(), t,
        C3DVector(0.0f, 0.0f, 0.0f));
    PreviousSpline.Tension = s * PreviousSpline.Tension + t * other.PreviousSpline.Tension;
    PreviousSpline.ControlPoints = GFInterpolateArray(PreviousSpline.GetControlPoints(),
        other.PreviousSpline.GetControlPoints(), t, C3DVector(0.0f, 0.0f, 0.0f));
    if (ThisSpline.ControlPoints.Size() != PreviousSpline.ControlPoints.Size())
        PreviousSpline = ThisSpline;
    OffsetOnlyAtStartup = t > 0.5f ? other.OffsetOnlyAtStartup : OffsetOnlyAtStartup;
    ResetEachFrame = t > 0.5f ? other.ResetEachFrame : ResetEachFrame;
    SplinePosOffsetLimits = GFInterpolateArray(SplinePosOffsetLimits, other.SplinePosOffsetLimits, t,
        C3DVector(0.0f, 0.0f, 0.0f));
    SplinePosOffsetSpeed = GFInterpolateArray(SplinePosOffsetSpeed, other.SplinePosOffsetSpeed, t,
        C3DVector(0.0f, 0.0f, 0.0f));
}
}
