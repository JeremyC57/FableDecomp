// NParticleEngine::CPSCSplineBase::CPSCSplineBase() -- particle spline component base: CPSCBase part, a cardinal spline seeded with two zero control points (tension 0.5, ends not connected), and empty random-offset / position-parameter data pointers
// Leaf: ??0CPSCSplineBase
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B21530, length 112 bytes.
// Callees: NParticleEngine::CPSCBase::CPSCBase @ 0x00AF2620, CCardinalSpline::CCardinalSpline(const C3DVector&, const C3DVector&,
//          float, bool) @ 0x00A56AA0 (manifest "CPSCSplineBase_Initialize"; it zeroes the control-point vector, stores tension and
//          the flag and pushes both points). Vtable 0x012A0B14 (NParticleEngine::CPSCSplineBase).
// Layout: retail CPSCBase is 0xc bytes; SplineAnimSpeed +0xc (left unset), Spline +0x10 (CCardinalSpline, retail 0x14: a
// 12-byte vector, Tension, EndPointsConnected), SplineRandomOffsetData +0x24, SplinePosParamData +0x2c (counted pointers).
// Shape: the spline is a mem-initialiser with two C3DVector(0,0,0) temporaries (built in the frame after the vtable
// store, between the argument pushes); the counted pointers' inline constructors zero their words afterwards.
namespace NParticleEngine {
class CParticleSystem;
struct CBase {
    virtual ~CBase();
};
class CPSCBase : public CBase { public:                   // vtable 0x0129F3F4
    CParticleSystem* ParticleSystem;        // +0x04
    bool             UpdateStarted : 1;     // +0x08 bit 0
    bool             Initialised : 1;       // +0x08 bit 1
    CPSCBase();                             // 0x00AF2620
};
}
#include "engine/C3DVector.h"
class CCardinalSpline { public:                    // retail 0x14 bytes
    C3DVector* m_first;                     // +0x00 ControlPoints (vector)
    C3DVector* m_last;                      // +0x04
    C3DVector* m_end;                       // +0x08
    float      Tension;                     // +0x0c
    bool       EndPointsConnected;          // +0x10
    CCardinalSpline(const C3DVector& p0, const C3DVector& p1, float tension, bool endPointsConnected);   // 0x00A56AA0
};
#include "engine/CCountedPointer.h"

namespace NParticleEngine {
class CPSCSplineBase : public CPSCBase { public:          // vtable 0x012A0B14
    class CSplineRandomOffsetData;
    class CSplinePosParamData;
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