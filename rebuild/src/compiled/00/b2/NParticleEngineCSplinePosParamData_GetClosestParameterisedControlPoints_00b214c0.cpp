// NParticleEngine::CSplinePosParamData::GetClosestParameterisedControlPoints(params, float pos, p0, p1, p2, p3) -- the control-point window for a parametric position: first = (long)(pos * lastIndex), last = first + 1 clamped at the last point, handed to the range overload; returns its result
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B214C0, length 97 bytes (`ret 18h`); called from TransformFromPos 0x00B21850 (at 0x00B2187D)
// and 0x00B2171B. Label correction: the catalog's "TransformFromPosition" is the float overload of GetClosestParameterisedControlPoints.
// Callees: the (params, first, last, p0..p3) overload @ 0x00B20D30 (catalog CSplinePosParamData::GetClosestParameterisedControlPoints, 8 params).
// Layout: Ego_r.pdb NParticleEngine::CSplinePosParamData (Spline +4, MaintainSplineShape +8, SplinePosParam +0xc) with the landed
// retail model of 0x00B20B10 / 0x00B21940 (CCardinalSpline = 12-byte CArray<C3DVector> + tension + flag).
// Shape: the last index is a named long (kept in esi and homed in the frame for `fild`), `pos * (float)lastIndex` truncated by __ftol2 (0x00BFEA70).
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
    bool GetClosestParameterisedControlPoints(CParticleUpdateParameters* params, float pos,
        C3DVector* p0, C3DVector* p1, C3DVector* p2, C3DVector* p3);
};

bool CSplinePosParamData::GetClosestParameterisedControlPoints(CParticleUpdateParameters* params, float pos,
    C3DVector* p0, C3DVector* p1, C3DVector* p2, C3DVector* p3)
{
    long lastIndex = Spline->ControlPoints.Size() - 1;
    long first = (long)(pos * (float)lastIndex);
    long last = first + 1 < lastIndex ? first + 1 : lastIndex;
    return GetClosestParameterisedControlPoints(params, first, last, p0, p1, p2, p3);
}

}