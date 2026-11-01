// NParticleEngine::CPSCSplineBase::CSplinePosParamData::TransformFromPosition(const CParticleUpdateParameters<unsigned long, unsigned long>& params, C3DVector& point, float pos) -- when keeping the spline shape: transform the point from the parameterised control points around the spline position
// Leaf: ?TransformFromPosition@CSplinePosParamData@CPSCSplineBase@NParticleEngine@@QAEXABV?$CParticleUpdateParameters@KK@3@AAVC3DVector@@M@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B21850, length 93 bytes (`ret 0Ch`).
// Name (naming wave 20, chunk 686): Ego_r / FableWin.pdb ?TransformFromPosition@CSplinePosParamData@CPSCSplineBase@NParticleEngine@@QAEXABV?$CParticleUpdateParameters@KK@3@AAVC3DVector@@M@Z
//   (three parameters; the queue's one-parameter spelling is in neither PDB) -- the sibling of the landed TransformFromIndex 0x00B218B0
//   (same frame of four C3DVectors, the index callee replaced by the float one); its callers GetPointAtPos 0x00B21940 bound it as
//   TransformFromPos. Was the propagated CEngineScreenEffectColourFilter::Render over ComputeCorners / DrawCorners stand-ins.
// Callees: the private GetClosestParameterisedControlPoints(params, pos, p0..p3) @ 0x00B214C0 (the M overload, the landed 0x00B216F0 leaf;
//   a bool in al) and the private TransformPosToMaintainShape(point, p0..p3) @ 0x00B202D0, both on `this`.
// Shape: a 0x30-byte frame of four C3DVectors pushed last-first with `lea` into eax/ecx/edx, each argument read from the stack once,
//   between the pushes; MaintainSplineShape (+8) tested first.
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

void CPSCSplineBase::CSplinePosParamData::TransformFromPosition(const CUpdateParams& params, C3DVector& point, float pos)
{
    if (MaintainSplineShape) {
        C3DVector p0, p1, p2, p3;
        if (GetClosestParameterisedControlPoints(params, pos, p0, p1, p2, p3))
            TransformPosToMaintainShape(point, p0, p1, p2, p3);
    }
}

}
