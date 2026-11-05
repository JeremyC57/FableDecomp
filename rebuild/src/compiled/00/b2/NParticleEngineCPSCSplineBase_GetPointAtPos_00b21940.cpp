// NParticleEngine::CPSCSplineBase::GetPointAtPos(const CParticleUpdateParameters<unsigned long, unsigned long>& params, C3DVector& out, float pos, float time) -- point on the spline at a parametric position (through the random-offset data when present), then transformed by the position parameters if any are bound
// Leaf: ?GetPointAtPos@CPSCSplineBase@NParticleEngine@@QAEXABV?$CParticleUpdateParameters@KK@2@AAVC3DVector@@MM@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B21940, length 82 bytes.
// Name (naming wave 20, chunk 686): Ego_r / FableWin.pdb spellings throughout -- the nested CPSCSplineBase::CSplineRandomOffsetData /
//   CSplinePosParamData, the template CParticleUpdateParameters<unsigned long, unsigned long> and the reference parameters (was the
//   un-nested pointer form binding 0x00B21850 as TransformFromPos).
// Callees: CSplineRandomOffsetData::GetPointAtPos @ 0x00B20FD0, CCardinalSpline::GetSplinePosition const @ 0x00A567A0,
//   CSplinePosParamData::TransformFromPosition @ 0x00B21850 (it sits between CSplinePosParamData::Update 0x00B20BA0 and TransformFromIndex 0x00B218B0).
// Layout: Ego_r.pdb CPSCSplineBase (SplineAnimSpeed +0xc, CCardinalSpline Spline +0x10, CCountedPointer<CSplineRandomOffsetData> +0x28,
//   CCountedPointer<CSplinePosParamData> +0x30) with the retail 12-byte CArray inside CCardinalSpline (0x14 bytes): the two counted
//   pointers ({Data, Info}) sit at +0x24 and +0x2c. CSplinePosParamData as the landed 0x00B20B10 (CArray SplinePosParam +0xc).
// Shape: the floats are forwarded as dwords from callee-saved registers; the bound-parameter test is
//   `Data && SplinePosParam.Size() > 0` (unsigned element count, `jbe`).
struct CBase {
    virtual ~CBase();
};
#include "engine/C3DVector.h"
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
#include "engine/CCountedPointer.h"
}
class CCardinalSpline { public:                                 // 0x14 bytes in retail
    NParticleEngine::CArray<C3DVector> ControlPoints;           // +0x00
    float             Tension;                                  // +0x0c
    bool              EndPointsConnected;                       // +0x10
    void GetSplinePosition(C3DVector& out, float pos) const;    // 0x00A567A0 ?GetSplinePosition@CCardinalSpline@@QBEXAAVC3DVector@@M@Z (Ego_r.pdb)
};
namespace NParticleEngine {
class CPSCBase : public CBase { public:                   // vtable 0x0129F3F4
    CParticleSystem* ParticleSystem;        // +0x04
    bool             UpdateStarted : 1;     // +0x08 bit 0
    bool             Initialised : 1;       // +0x08 bit 1
};
class CPSCSplineBase : public CPSCBase { public:
    class CSplineRandomOffsetData { public:                     // nested in CPSCSplineBase (Ego_r.pdb)
        void OffsetSplinePositions();                                       // 0x00B206B0
        void GetPointAtPos(C3DVector& out, float pos, float time);          // 0x00B20FD0 ?GetPointAtPos@CSplineRandomOffsetData@CPSCSplineBase@NParticleEngine@@QAEXAAVC3DVector@@MM@Z (Ego_r.pdb)
        void GetPointAtIndex(C3DVector& out, long index, float time);       // 0x00B21090 ?GetPointAtIndex@CSplineRandomOffsetData@CPSCSplineBase@NParticleEngine@@QAEXAAVC3DVector@@JM@Z
    };
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
    float                                     SplineAnimSpeed;        // +0x0c
    CCardinalSpline                           Spline;                 // +0x10
    CCountedPointer<CSplineRandomOffsetData>  SplineRandomOffsetData; // +0x24
    CCountedPointer<CSplinePosParamData>      SplinePosParamData;     // +0x2c
    bool Initialise(void* owner, unsigned long flags, const CUpdateParams& params, float time);
    void GetPointAtPos(const CUpdateParams& params, C3DVector& out, float pos, float time);      // ?GetPointAtPos@CPSCSplineBase@NParticleEngine@@QAEXABV?$CParticleUpdateParameters@KK@2@AAVC3DVector@@MM@Z
    void GetPointAtIndex(const CUpdateParams& params, C3DVector& out, long index, float time);   // ?GetPointAtIndex@CPSCSplineBase@NParticleEngine@@QAEXABV?$CParticleUpdateParameters@KK@2@AAVC3DVector@@JM@Z
};

void CPSCSplineBase::GetPointAtPos(const CUpdateParams& params, C3DVector& out, float pos, float time)
{
    if (SplineRandomOffsetData.Data)
        SplineRandomOffsetData->GetPointAtPos(out, pos, time);
    else
        Spline.GetSplinePosition(out, pos);
    if (SplinePosParamData.Data && SplinePosParamData->SplinePosParam.Size() > 0)
        SplinePosParamData->TransformFromPosition(params, out, pos);
}
}
