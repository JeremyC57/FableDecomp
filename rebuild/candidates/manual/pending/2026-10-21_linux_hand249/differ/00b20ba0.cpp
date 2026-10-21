// CSplinePosParamData::Update(CParticleUpdateParameters&, CCardinalSpline*) -- copy each particle-tracked position (inverse-transformed into system space) onto its spline control point
// Grade: DIFFER(ours v retail) -- 397v391: frame 0x64 v 0x58; retail copies the C3DVector with integer moves and keeps the transformed X on the x87 stack into a second copy; ours copies with fld/fst
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
};
struct CMatrix3x4 {
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
    __forceinline C3DVector Transform(const C3DVector& v) const
    {
        C3DVector r;
        r.X = v.X * E11 + v.Y * E21 + v.Z * E31 + E41;
        r.Y = v.X * E12 + v.Y * E22 + v.Z * E32 + E42;
        r.Z = v.X * E13 + v.Y * E23 + v.Z * E33 + E43;
        return r;
    }
};
template<class T> struct CArray {
    T* m_begin; T* m_end; T* m_capacity;
    long Size() const { return (long)(m_end - m_begin); }
    T& operator[](long i) { return m_begin[i]; }
};
struct CCardinalSpline {
    CArray<C3DVector> ControlPoints;               // +0x00
    float Tension;
    bool EndPointsConnected;
};
struct CParticleSystem {
    void GetInverseSystemTransformWithTranslate(CMatrix3x4* out);   // 0x00B201D0
};
struct SPositionEntry { unsigned long key; C3DVector value; };
struct CPositionMap {                              // CVectorMap<unsigned long, C3DVector>
    SPositionEntry* first;
    SPositionEntry* last;
    SPositionEntry* end;
    SPositionEntry* LowerBound(const unsigned long* key);   // 0x00B05890
};
struct CParticleUpdateParameters {
    void*        _vtbl;                            // +0x00
    CPositionMap Positions;                        // +0x04
};
struct CSplinePosParamData {
    CParticleSystem*       PParentParticleSystem;  // +0x00
    CCardinalSpline*       Spline;                 // +0x04
    bool                   MaintainSplineShape;    // +0x08
    CArray<unsigned long>  SplinePosParam;         // +0x0c
    void Update(CParticleUpdateParameters& params, CCardinalSpline* spline);
};

void CSplinePosParamData::Update(CParticleUpdateParameters& params, CCardinalSpline* spline)
{
    if (MaintainSplineShape)
        return;
    for (long i = 0; i < Spline->ControlPoints.Size(); ++i) {
        unsigned long* pid = &SplinePosParam[i];
        unsigned long id = *pid;
        if (id) {
            CPositionMap* map = &params.Positions;
            SPositionEntry* it = map->LowerBound(pid);
            if (it == map->last || id < it->key)
                it = map->last;
            if (it != params.Positions.last) {
                C3DVector pos = it->value;
                C3DVector result = pos;
                if (PParentParticleSystem) {
                    CMatrix3x4 m;
                    PParentParticleSystem->GetInverseSystemTransformWithTranslate(&m);
                    result = m.Transform(pos);
                }
                spline->ControlPoints[i] = result;
            }
        }
    }
}
