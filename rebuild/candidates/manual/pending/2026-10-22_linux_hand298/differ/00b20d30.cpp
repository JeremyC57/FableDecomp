// Grade: DIFFER(563v578) -- retail's two search loops are neither rotated nor strength-reduced (top test, re-read of SplinePosParam's data pointer each pass, a jmp back to an aligned top); every while/for/goto form tried here gets rotated with a pointer induction variable. The point transform also differs: retail copies the by-value vector to locals and sums the z, y, x terms in that order
// NParticleEngine::CSplinePosParamData::GetClosestParameterisedControlPoints(params, long first, long last, p0, p1, p2, p3) -- widen [first, last] outwards to the nearest control points bound to a particle parameter, return those control points and their bound parameter values (taken into the particle system's space when there is one)
// Leaf: GetClosestParameterisedControlPoints
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B20D30, 578 bytes, `ret 1Ch`. Called from the landed index/float overloads
//   0x00B21450 / 0x00B214C0.
// Callees: CParticleUpdateParameters<ulong,ulong> lookup 0x00B059F0 (catalog "CVectorMap_LookupAndCopyVolume"; (out, key&)
//   -> bool), CParticleSystem::GetInverseSystemTransformWithTranslate 0x00B201D0 (landed).
// Layout: Ego_r.pdb NParticleEngine::CSplinePosParamData (PParentParticleSystem +0, Spline +4, MaintainSplineShape +8,
//   SplinePosParam +0xc) with the landed retail CCardinalSpline (12-byte CArray<C3DVector> first); CMatrix3x4 = twelve floats
//   (rows, then the translation) as the landed 0x00B201D0.
namespace NParticleEngine {
struct C3DVector { float X, Y, Z; };
struct CMatrix3x4 { float m[12]; };
__forceinline C3DVector operator*(C3DVector v, const CMatrix3x4& t)
{
    C3DVector r;
    r.X = v.X * t.m[0] + v.Y * t.m[3] + v.Z * t.m[6] + t.m[9];
    r.Y = v.X * t.m[1] + v.Y * t.m[4] + v.Z * t.m[7] + t.m[10];
    r.Z = v.X * t.m[2] + v.Y * t.m[5] + v.Z * t.m[8] + t.m[11];
    return r;
}
template<class T> struct CArray {
    T* m_pData;
    T* m_pLast;
    T* m_pEnd;
};
struct CCardinalSpline {
    CArray<C3DVector> ControlPoints;
    float             Tension;
    bool              EndPointsConnected;
};
struct CParticleSystem {
    void GetInverseSystemTransformWithTranslate(CMatrix3x4* result);   // 0x00B201D0
};
struct CParticleUpdateParameters {
    bool GetValue(C3DVector* out, const unsigned long& key);           // 0x00B059F0
};
struct CSplinePosParamData {
    CParticleSystem*      PParentParticleSystem;
    CCardinalSpline*      Spline;
    bool                  MaintainSplineShape;
    CArray<unsigned long> SplinePosParam;
    bool GetClosestParameterisedControlPoints(CParticleUpdateParameters* params, long first, long last,
        C3DVector* p0, C3DVector* p1, C3DVector* p2, C3DVector* p3);
};
bool CSplinePosParamData::GetClosestParameterisedControlPoints(CParticleUpdateParameters* params, long first, long last,
    C3DVector* p0, C3DVector* p1, C3DVector* p2, C3DVector* p3)
{
findFirst:
    if (first < 0)
        return false;
    if (SplinePosParam.m_pData[first] == 0) {
        --first;
        goto findFirst;
    }
    long count = Spline->ControlPoints.m_pLast - Spline->ControlPoints.m_pData;
findLast:
    if (last >= count)
        return false;
    if (SplinePosParam.m_pData[last] == 0) {
        ++last;
        goto findLast;
    }
    bool ok = params->GetValue(p1, SplinePosParam.m_pData[first]);
    ok &= params->GetValue(p3, SplinePosParam.m_pData[last]);
    if (ok) {
        *p0 = Spline->ControlPoints.m_pData[first];
        *p2 = Spline->ControlPoints.m_pData[last];
        if (PParentParticleSystem) {
            CMatrix3x4 transform;
            PParentParticleSystem->GetInverseSystemTransformWithTranslate(&transform);
            *p1 = *p1 * transform;
            *p3 = *p3 * transform;
        }
    }
    return ok;
}
}
