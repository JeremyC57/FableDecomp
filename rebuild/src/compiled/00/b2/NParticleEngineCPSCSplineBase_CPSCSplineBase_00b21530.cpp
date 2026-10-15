// NParticleEngine::CPSCSplineBase::CPSCSplineBase() -- particle spline component base: CPSCBase part, a cardinal spline seeded with two zero control points (tension 0.5, ends not connected), and empty random-offset / position-parameter data pointers
// Leaf: ??0CPSCSplineBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B21530, length 112 bytes.
// Callees: NParticleEngine::CPSCBase::CPSCBase @ 0x00AF2620, CCardinalSpline::CCardinalSpline(const C3DVector&, const C3DVector&,
//          float, bool) @ 0x00A56AA0 (manifest "CPSCSplineBase_Initialize"; it zeroes the control-point vector, stores tension and
//          the flag and pushes both points). Vtable 0x012A0B14 (NParticleEngine::CPSCSplineBase).
// Layout: retail CPSCBase is 0xc bytes; SplineAnimSpeed +0xc (left unset), Spline +0x10 (CCardinalSpline, retail 0x14: a
// 12-byte vector, Tension, EndPointsConnected), SplineRandomOffsetData +0x24, SplinePosParamData +0x2c (counted pointers).
// Shape: the spline is a mem-initialiser with two C3DVector(0,0,0) temporaries (built in the frame after the vtable
// store, between the argument pushes); the counted pointers' inline constructors zero their words afterwards.
namespace NParticleEngine {
struct CParticleSystem;
struct CBase {
    virtual ~CBase();
};
struct CPSCBase : CBase {                   // vtable 0x0129F3F4
    CParticleSystem* ParticleSystem;        // +0x04
    bool             UpdateStarted : 1;     // +0x08 bit 0
    bool             Initialised : 1;       // +0x08 bit 1
    CPSCBase();                             // 0x00AF2620
};
}
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
struct CCardinalSpline {                    // retail 0x14 bytes
    C3DVector* m_first;                     // +0x00 ControlPoints (vector)
    C3DVector* m_last;                      // +0x04
    C3DVector* m_end;                       // +0x08
    float      Tension;                     // +0x0c
    bool       EndPointsConnected;          // +0x10
    CCardinalSpline(const C3DVector& p0, const C3DVector& p1, float tension, bool endPointsConnected);   // 0x00A56AA0
};
template <class T> struct CCountedPointer {
    T*    Data;
    void* Info;
    CCountedPointer() : Data(0), Info(0) {}
};

namespace NParticleEngine {
struct CPSCSplineBase : CPSCBase {          // vtable 0x012A0B14
    struct CSplineRandomOffsetData;
    struct CSplinePosParamData;
    float                                    SplineAnimSpeed;           // +0x0c
    CCardinalSpline                          Spline;                    // +0x10
    CCountedPointer<CSplineRandomOffsetData> SplineRandomOffsetData;    // +0x24
    CCountedPointer<CSplinePosParamData>     SplinePosParamData;        // +0x2c
    virtual ~CPSCSplineBase();
    CPSCSplineBase();
};

CPSCSplineBase::CPSCSplineBase()
    : Spline(C3DVector(0.0f, 0.0f, 0.0f), C3DVector(0.0f, 0.0f, 0.0f), 0.5f, false)
{
}
}