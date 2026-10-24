// NParticleEngine::CPSCSplineBase::CSplinePosParamData::GetScaleFactor(const CParticleUpdateParameters<unsigned long,unsigned long>& params, float pos) -- with MaintainSplineShape set, the ratio of the current to the original length of the spline segment around `pos`; 1.0 otherwise or when no control points bracket it
// Leaf: ?GetScaleFactor@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B216F0, length 177 bytes, `ret 8`, the float in st(0).
// Signature from Ego_r.pdb: ?GetScaleFactor@CSplinePosParamData@CPSCSplineBase@NParticleEngine@@QAEMABV?$CParticleUpdate
//   Parameters@KK@3@M@Z. Caller: the landed CPSCSpline::StartUpdate @ 0x00B1B0C0 (call 0x00B1B111, there named
//   "CalcDistanceAlpha").
// Callees: CSplinePosParamData::GetClosestParameterisedControlPoints(params, float, C3DVector&, C3DVector&, C3DVector&,
//   C3DVector&) @ 0x00B214C0 (private, returns bool; the float overload of the three in the PDB).
// Layout (Ego_r.pdb): CSplinePosParamData = PParentParticleSystem +0, Spline +4, bool MaintainSplineShape +8, ...
// Shape: four C3DVector locals filled by the bool query; the result is |c - a| / |d - b| (the two differences computed
//   component by component on the FPU, each length an inline sqrt of the dot product, the first one parked in the dead
//   `pos` argument slot); both failure paths return the 1.0f literal.
#include <math.h>

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    float GetLength() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
};

class CCardinalSpline;

namespace NParticleEngine {

template <class A, class B> class CParticleUpdateParameters;
class CParticleSystem;

class CPSCSplineBase {
public:
    class CSplinePosParamData {
    public:
        CParticleSystem* PParentParticleSystem;   // +0x00
        CCardinalSpline* Spline;                  // +0x04
        bool             MaintainSplineShape;     // +0x08

        float GetScaleFactor(const CParticleUpdateParameters<unsigned long, unsigned long>& params, float pos);
    private:
        bool GetClosestParameterisedControlPoints(const CParticleUpdateParameters<unsigned long, unsigned long>& params,
                                                  float pos, C3DVector& originalStart, C3DVector& originalEnd,
                                                  C3DVector& currentStart, C3DVector& currentEnd);   // 0x00B214C0
    };
};

float CPSCSplineBase::CSplinePosParamData::GetScaleFactor(const CParticleUpdateParameters<unsigned long, unsigned long>& params, float pos)
{
    if (MaintainSplineShape) {
        C3DVector originalStart, originalEnd, currentStart, currentEnd;
        if (GetClosestParameterisedControlPoints(params, pos, originalStart, originalEnd, currentStart, currentEnd)) {
            C3DVector current = currentEnd - originalEnd;
            C3DVector original = currentStart - originalStart;
            return original.GetLength() / current.GetLength();
        }
    }
    return 1.0f;
}

}   // namespace NParticleEngine