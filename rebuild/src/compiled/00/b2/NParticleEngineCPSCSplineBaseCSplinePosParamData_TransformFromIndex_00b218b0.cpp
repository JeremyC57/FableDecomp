// NParticleEngine::CPSCSplineBase::CSplinePosParamData::TransformFromIndex(const CParticleUpdateParameters<unsigned long, unsigned long>& params, C3DVector& point, long index) -- when keeping the spline shape: use the particle parameter bound to this control point when there is one, else transform the point from the surrounding parameterised control points
// Leaf: ?TransformFromIndex@CSplinePosParamData@CPSCSplineBase@NParticleEngine@@QAEXABV?$CParticleUpdateParameters@KK@3@AAVC3DVector@@J@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B218B0, length 130 bytes (`ret 0Ch`).
// Name (naming wave 20, chunk 686): Ego_r / FableWin.pdb -- CSplinePosParamData is nested in CPSCSplineBase, the parameters are the
//   CParticleUpdateParameters<unsigned long, unsigned long> const reference and the C3DVector reference (was the un-nested pointer spelling).
// Callees: CParticleUpdateParameters<KK>::GetVectorParam @ 0x00B059F0 (landed; (vector&, key&) -> bool), the private
//   GetClosestParameterisedControlPoints(params, index, p0..p3) @ 0x00B21450 and the private TransformPosToMaintainShape @ 0x00B202D0 on `this`.
// Layout: as 0x00B217D0 (MaintainSplineShape +8, SplinePosParam +0xc).
// Shape: the bound key is a named local (VC homes it in the dead `index` slot for the by-reference pass); the early `return`
// after a successful lookup, four C3DVector locals declared after it.
class C3DVector { public: float X, Y, Z; };                    // global (Ego_r.pdb class V; 1-aligned in retail)
class CCardinalSpline;                                          // global (Ego_r.pdb class V)
namespace NParticleEngine {
class CParticleSystem;
template <class T> class CArray { public:                       // retail CArray<T>: three pointers (Ego_r.pdb class V)
    T* m_pData;
    T* m_pLast;
    T* m_pEnd;
    T* GetData() const { return m_pData; }
    unsigned long Size() const { return (unsigned long)(m_pLast - m_pData); }
};
template <class TKey, class TParam> class CParticleUpdateParameters { public:   // CParticleUpdateParameters<unsigned long, unsigned long>
    bool GetVectorParam(C3DVector& vector, const unsigned long& key) const;   // 0x00B059F0 ?GetVectorParam@?$CParticleUpdateParameters@KK@NParticleEngine@@QBE_NAAVC3DVector@@ABK@Z (Ego_r.pdb, the landed leaf)
};
typedef CParticleUpdateParameters<unsigned long, unsigned long> CUpdateParams;
class CPSCSplineBase { public:
    class CSplinePosParamData { public:                         // nested in CPSCSplineBase (Ego_r / FableWin.pdb)
        CParticleSystem*      PParentParticleSystem;    // +0x00
        CCardinalSpline*      Spline;                   // +0x04
        bool                  MaintainSplineShape;      // +0x08
        CArray<unsigned long> SplinePosParam;           // +0x0c
        void TransformFromPosition(const CUpdateParams& params, C3DVector& point, float pos);   // 0x00B21850 ?TransformFromPosition@CSplinePosParamData@CPSCSplineBase@NParticleEngine@@QAEXABV?$CParticleUpdateParameters@KK@3@AAVC3DVector@@M@Z
        void TransformFromIndex(const CUpdateParams& params, C3DVector& point, long index);     // 0x00B218B0 ?TransformFromIndex@CSplinePosParamData@CPSCSplineBase@NParticleEngine@@QAEXABV?$CParticleUpdateParameters@KK@3@AAVC3DVector@@J@Z
    private:
        bool GetClosestParameterisedControlPoints(const CUpdateParams& params, long index, C3DVector& p0, C3DVector& p1, C3DVector& p2, C3DVector& p3);   // 0x00B21450 (Ego_r.pdb private, the J overload)
        bool GetClosestParameterisedControlPoints(const CUpdateParams& params, float pos, C3DVector& p0, C3DVector& p1, C3DVector& p2, C3DVector& p3);    // 0x00B214C0 (Ego_r.pdb private, the M overload; the landed 0x00B216F0 leaf)
        void TransformPosToMaintainShape(C3DVector& point, const C3DVector& p0, const C3DVector& p1, const C3DVector& p2, const C3DVector& p3);          // 0x00B202D0 ?TransformPosToMaintainShape@CSplinePosParamData@CPSCSplineBase@NParticleEngine@@AAEXAAVC3DVector@@ABV4@111@Z (Ego_r / FableWin.pdb private)
    };
};

void CPSCSplineBase::CSplinePosParamData::TransformFromIndex(const CUpdateParams& params, C3DVector& point, long index)
{
    if (MaintainSplineShape) {
        unsigned long key = SplinePosParam.m_pData[index];
        if (key && params.GetVectorParam(point, key))
            return;
        C3DVector p0, p1, p2, p3;
        if (GetClosestParameterisedControlPoints(params, index, p0, p1, p2, p3))
            TransformPosToMaintainShape(point, p0, p1, p2, p3);
    }
}

}
