// NParticleEngine::CSplinePosParamData::TransformFromIndex(CParticleUpdateParameters* params, C3DVector* point, long index) -- when keeping the spline shape: use the particle parameter bound to this control point when there is one, else transform the point from the surrounding parameterised control points
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B218B0, length 130 bytes (`ret 0Ch`).
// Callees: the parameter lookup @ 0x00B059F0 on params (catalog "CVectorMap_LookupAndCopyVolume"; (out, key&) -> bool),
// GetClosestParameterisedControlPoints(params, index, p0..p3) @ 0x00B21450 (this chunk), the four-point transform @ 0x00B202D0
// (catalog "Transform_VectorWithQuaternion") on `this`.
// Layout: as 0x00B217D0 (MaintainSplineShape +8, SplinePosParam +0xc).
// Shape: the bound key is a named local (VC homes it in the dead `index` slot for the by-reference pass); the early `return`
// after a successful lookup, four C3DVector locals declared after it.
// Leaf: TransformFromIndex
namespace NParticleEngine {
class CParticleSystem;
struct CCardinalSpline;
struct C3DVector { float X, Y, Z; };

template<class T> struct CArray {                    // retail CArray<T>: three pointers
    T* m_pData;
    T* m_pLast;
    T* m_pEnd;
};

struct CParticleUpdateParameters {                   // CParticleUpdateParameters<unsigned long, unsigned long>
    bool GetValue(C3DVector* out, const unsigned long& key);   // 0x00B059F0 (catalog "CVectorMap_LookupAndCopyVolume")
};

struct CSplinePosParamData {
    CParticleSystem*      PParentParticleSystem;    // +0x00
    CCardinalSpline*      Spline;                   // +0x04
    bool                  MaintainSplineShape;      // +0x08
    CArray<unsigned long> SplinePosParam;           // +0x0c
    bool GetClosestParameterisedControlPoints(CParticleUpdateParameters* params, long index,
        C3DVector* p0, C3DVector* p1, C3DVector* p2, C3DVector* p3);   // 0x00B21450
    void TransformPoint(C3DVector* point, C3DVector* p0, C3DVector* p1, C3DVector* p2, C3DVector* p3);   // 0x00B202D0
    void TransformFromIndex(CParticleUpdateParameters* params, C3DVector* point, long index);
};

void CSplinePosParamData::TransformFromIndex(CParticleUpdateParameters* params, C3DVector* point, long index)
{
    if (MaintainSplineShape) {
        unsigned long key = SplinePosParam.m_pData[index];
        if (key && params->GetValue(point, key))
            return;
        C3DVector p0, p1, p2, p3;
        if (GetClosestParameterisedControlPoints(params, index, &p0, &p1, &p2, &p3))
            TransformPoint(point, &p0, &p1, &p2, &p3);
    }
}

}