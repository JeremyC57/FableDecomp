# h20_spline.py: naming wave 20, family spline -- the NParticleEngine::CPSCSplineBase::CSplinePosParamData rows re-modelled on the PDB
# (nested class, template CParticleUpdateParameters<unsigned long, unsigned long>, global C3DVector / CCardinalSpline, reference parameters;
# byte-identical only):
#   00b218b0 ?TransformFromIndex@CSplinePosParamData@CPSCSplineBase@NParticleEngine@@QAEXABV?$CParticleUpdateParameters@KK@3@AAVC3DVector@@J@Z
#            (own leaf was the un-nested pointer form; callees GetVectorParam 0x00B059F0 (the landed leaf), the private
#            GetClosestParameterisedControlPoints (long) 0x00B21450 and TransformPosToMaintainShape 0x00B202D0 -- the c686 queue row);
#   00b21850 ?TransformFromPosition@CSplinePosParamData@CPSCSplineBase@NParticleEngine@@QAEXABV?$CParticleUpdateParameters@KK@3@AAVC3DVector@@M@Z
#            (Ego_r / FableWin.pdb: three parameters, not the queue's one; was the propagated CEngineScreenEffectColourFilter::Render over
#            ComputeCorners / DrawCorners stand-ins; callees the private (float) GetClosestParameterisedControlPoints 0x00B214C0 and 0x00B202D0);
#   00b21940 ?GetPointAtPos@CPSCSplineBase@NParticleEngine@@QAEXABV?$CParticleUpdateParameters@KK@2@AAVC3DVector@@MM@Z (binds 0x00B21850 as
#            TransformFromPos; callees ?GetPointAtPos@CSplineRandomOffsetData@CPSCSplineBase@NParticleEngine@@QAEXAAVC3DVector@@MM@Z 0x00B20FD0,
#            ?GetSplinePosition@CCardinalSpline@@QBEXAAVC3DVector@@M@Z 0x00A567A0);
#   00b219a0 ?GetPointAtIndex@CPSCSplineBase@NParticleEngine@@QAEXABV?$CParticleUpdateParameters@KK@2@AAVC3DVector@@JM@Z (binds 0x00B218B0 under
#            the un-nested spelling; callee ?GetPointAtIndex@CSplineRandomOffsetData@CPSCSplineBase@NParticleEngine@@QAEXAAVC3DVector@@JM@Z 0x00B21090).
import os, sys, re
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he
_X = set()
_xp = os.path.join(he.W, 'runs', 'excl_spline.txt')
if os.path.exists(_xp): _X = set(l.split()[0] for l in open(_xp) if l.strip())

HEAD = '''class C3DVector { public: float X, Y, Z; };                    // global (Ego_r.pdb class V; 1-aligned in retail)
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
'''
POSDATA = '''class CSplinePosParamData { public:                         // nested in CPSCSplineBase (Ego_r / FableWin.pdb)
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
'''
TFI = '''// NParticleEngine::CPSCSplineBase::CSplinePosParamData::TransformFromIndex(const CParticleUpdateParameters<unsigned long, unsigned long>& params, C3DVector& point, long index) -- when keeping the spline shape: use the particle parameter bound to this control point when there is one, else transform the point from the surrounding parameterised control points
// Leaf: ?TransformFromIndex@CSplinePosParamData@CPSCSplineBase@NParticleEngine@@QAEXABV?$CParticleUpdateParameters@KK@3@AAVC3DVector@@J@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B218B0, length 130 bytes (`ret 0Ch`).
// Name (naming wave 20, chunk 686): Ego_r / FableWin.pdb -- CSplinePosParamData is nested in CPSCSplineBase, the parameters are the
//   CParticleUpdateParameters<unsigned long, unsigned long> const reference and the C3DVector reference (was the un-nested pointer spelling).
// Callees: CParticleUpdateParameters<KK>::GetVectorParam @ 0x00B059F0 (landed; (vector&, key&) -> bool), the private
//   GetClosestParameterisedControlPoints(params, index, p0..p3) @ 0x00B21450 and the private TransformPosToMaintainShape @ 0x00B202D0 on `this`.
// Layout: as 0x00B217D0 (MaintainSplineShape +8, SplinePosParam +0xc).
// Shape: the bound key is a named local (VC homes it in the dead `index` slot for the by-reference pass); the early `return`
// after a successful lookup, four C3DVector locals declared after it.
''' + HEAD + 'class CPSCSplineBase { public:\n    ' + POSDATA + '''};

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
'''
TFP = '''// NParticleEngine::CPSCSplineBase::CSplinePosParamData::TransformFromPosition(const CParticleUpdateParameters<unsigned long, unsigned long>& params, C3DVector& point, float pos) -- when keeping the spline shape: transform the point from the parameterised control points around the spline position
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
''' + HEAD + 'class CPSCSplineBase { public:\n    ' + POSDATA + '''};

void CPSCSplineBase::CSplinePosParamData::TransformFromPosition(const CUpdateParams& params, C3DVector& point, float pos)
{
    if (MaintainSplineShape) {
        C3DVector p0, p1, p2, p3;
        if (GetClosestParameterisedControlPoints(params, pos, p0, p1, p2, p3))
            TransformPosToMaintainShape(point, p0, p1, p2, p3);
    }
}

}
'''
BASE = '''struct CBase {
    virtual ~CBase();
};
''' + HEAD + '''class CCPPointerInfo;
}
template <class T> class CCountedPointer { public:              // global (Ego_r.pdb class V template)
    T*                              Data;
    NParticleEngine::CCPPointerInfo* Info;
    T* operator->() const { return Data; }
};
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
    ''' + POSDATA + '''    float                                     SplineAnimSpeed;        // +0x0c
    CCardinalSpline                           Spline;                 // +0x10
    CCountedPointer<CSplineRandomOffsetData>  SplineRandomOffsetData; // +0x24
    CCountedPointer<CSplinePosParamData>      SplinePosParamData;     // +0x2c
    bool Initialise(void* owner, unsigned long flags, const CUpdateParams& params, float time);
    void GetPointAtPos(const CUpdateParams& params, C3DVector& out, float pos, float time);      // ?GetPointAtPos@CPSCSplineBase@NParticleEngine@@QAEXABV?$CParticleUpdateParameters@KK@2@AAVC3DVector@@MM@Z
    void GetPointAtIndex(const CUpdateParams& params, C3DVector& out, long index, float time);   // ?GetPointAtIndex@CPSCSplineBase@NParticleEngine@@QAEXABV?$CParticleUpdateParameters@KK@2@AAVC3DVector@@JM@Z
};
'''
GPP = '''// NParticleEngine::CPSCSplineBase::GetPointAtPos(const CParticleUpdateParameters<unsigned long, unsigned long>& params, C3DVector& out, float pos, float time) -- point on the spline at a parametric position (through the random-offset data when present), then transformed by the position parameters if any are bound
// Leaf: ?GetPointAtPos@CPSCSplineBase@NParticleEngine@@QAEXABV?$CParticleUpdateParameters@KK@2@AAVC3DVector@@MM@Z
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
''' + BASE + '''
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
'''
GPI = '''// NParticleEngine::CPSCSplineBase::GetPointAtIndex(const CParticleUpdateParameters<unsigned long, unsigned long>& params, C3DVector& out, long index, float time) -- control point `index` of the spline (through the random-offset data when present), then transformed by the position parameters if any are bound
// Leaf: ?GetPointAtIndex@CPSCSplineBase@NParticleEngine@@QAEXABV?$CParticleUpdateParameters@KK@2@AAVC3DVector@@JM@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B219A0, length 86 bytes.
// Name (naming wave 20, chunk 686): Ego_r / FableWin.pdb spellings throughout (as GetPointAtPos 0x00B21940; was the un-nested pointer
//   form binding 0x00B218B0 as the un-nested TransformFromIndex).
// Retry 10b (getsw sweep, FIXES B-15): the data block is read through an inline getter `GetData()`; the plain member read gives the parked register/scheduling difference.
// Callees: CSplineRandomOffsetData::GetPointAtIndex @ 0x00B21090, CSplinePosParamData::TransformFromIndex @ 0x00B218B0.
// Layout: as GetPointAtPos 0x00B21940.
// Tried: plain POD assignment from the array element (this file), CArray operator[], a by-value
// GetControlPoint accessor (all three give the same allocation) and a user C3DVector::operator=
// (drops the `mov` and re-colours the callee-saved registers, 84 bytes).
''' + BASE + '''
void CPSCSplineBase::GetPointAtIndex(const CUpdateParams& params, C3DVector& out, long index, float time)
{
    if (SplineRandomOffsetData.Data)
        SplineRandomOffsetData->GetPointAtIndex(out, index, time);
    else
        out = Spline.ControlPoints.GetData()[index];
    if (SplinePosParamData.Data)
        SplinePosParamData->TransformFromIndex(params, out, index);
}
}
'''
for a, txt in (('00b218b0', TFI), ('00b21850', TFP), ('00b21940', GPP), ('00b219a0', GPI)):
    if a in _X: continue
    he.put(a, 'src', txt)
if '00b21850' not in _X:
    he.resub('00b21850', 'tst', r'\bCEngineScreenEffectColourFilter\b', 'CSplinePosParamData')
    he.resub('00b21850', 'tst', r'\bComputeCorners\b', 'GetClosestParameterisedControlPoints')
    he.resub('00b21850', 'tst', r'\bDrawCorners\b', 'TransformPosToMaintainShape')
    he.resub('00b21850', 'tst', r'\bRender\b', 'TransformFromPosition')
for a in ('00b21940',):
    if a not in _X: he.resub(a, 'tst', r'\bTransformFromPos\b', 'TransformFromPosition')
