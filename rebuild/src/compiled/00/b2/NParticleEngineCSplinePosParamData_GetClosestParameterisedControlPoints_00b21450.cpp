// NParticleEngine::CSplinePosParamData::GetClosestParameterisedControlPoints(params, long index, p0, p1, p2, p3) -- the control-point window around index (index-1 clamped at 0 .. index+1 clamped at the last point) handed to the range overload; returns its result
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B21450, length 97 bytes (`ret 18h`); called by TransformFromIndex 0x00B218B0.
// Callees: the (params, first, last, p0..p3) overload @ 0x00B20D30 (catalog CSplinePosParamData::GetClosestParameterisedControlPoints, 8 params).
// Layout: Ego_r.pdb NParticleEngine::CSplinePosParamData (Spline +4, MaintainSplineShape +8, SplinePosParam +0xc) with the landed
// retail model of 0x00B20B10 / 0x00B21940 (CCardinalSpline = 12-byte CArray<C3DVector> + tension + flag).
// Shape: `index - 1 > 0 ? index - 1 : 0` is VC's branchless `setle ; dec ; and`; the upper clamp a signed compare and branch.
// Leaf: GetClosestParameterisedControlPoints
namespace NParticleEngine {
class CParticleSystem;
struct CParticleUpdateParameters;           // CParticleUpdateParameters<unsigned long, unsigned long>

struct C3DVector {
    float X, Y, Z;
};

template<class T> struct CArray {           // retail CArray<T>: three pointers
    T* m_pData;
    T* m_pLast;
    T* m_pEnd;
    unsigned long Size() const { return (unsigned long)(m_pLast - m_pData); }
};

struct CCardinalSpline {                    // 0x14 bytes in retail
    CArray<C3DVector> ControlPoints;        // +0x00
    float             Tension;              // +0x0c
    bool              EndPointsConnected;   // +0x10
};

struct CSplinePosParamData {
    CParticleSystem*      PParentParticleSystem;    // +0x00
    CCardinalSpline*      Spline;                   // +0x04
    bool                  MaintainSplineShape;      // +0x08
    CArray<unsigned long> SplinePosParam;           // +0x0c
    bool GetClosestParameterisedControlPoints(CParticleUpdateParameters* params, long first, long last,
        C3DVector* p0, C3DVector* p1, C3DVector* p2, C3DVector* p3);    // 0x00B20D30
    bool GetClosestParameterisedControlPoints(CParticleUpdateParameters* params, long index,
        C3DVector* p0, C3DVector* p1, C3DVector* p2, C3DVector* p3);
};

bool CSplinePosParamData::GetClosestParameterisedControlPoints(CParticleUpdateParameters* params, long index,
    C3DVector* p0, C3DVector* p1, C3DVector* p2, C3DVector* p3)
{
    long lastIndex = Spline->ControlPoints.Size() - 1;
    long first = index - 1 > 0 ? index - 1 : 0;
    long last = index + 1 < lastIndex ? index + 1 : lastIndex;
    return GetClosestParameterisedControlPoints(params, first, last, p0, p1, p2, p3);
}

}