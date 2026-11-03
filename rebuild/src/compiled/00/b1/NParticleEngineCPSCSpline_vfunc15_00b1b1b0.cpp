// NParticleEngine::CPSCSpline::ReadBinary(CDataInputStream& in) -- vtable slot 15: read SplineBounce, ScaleSplineSpeed and SplineAnimSpeed back, then the CPSCSplineBase part
// Leaf: ?ReadBinary@CPSCSpline@NParticleEngine@@UAEXAAVCDataInputStream@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B1B1B0, thiscall, `ret 4`.
// Identity: FableWin.pdb / Ego_r.pdb ?ReadBinary@CPSCSpline@NParticleEngine@@UAEXAAVCDataInputStream@@@Z; slot 15 of the CPSCSpline
//   vtable 0x012A0934 (rtti-vtables.tsv; catalog "vfunc_15"); the mirror of the landed CPSCSpline::WriteBinary 0x00B1AFF0.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (the inlined Read's slow path), CPSCSplineBase::ReadBinary
//   0x00B22B40 (FableWin ?ReadBinary@CPSCSplineBase@NParticleEngine@@UAEXAAVCDataInputStream@@@Z, slot 15 of 0x012A0B14, called
//   non-virtually last). Inlined: CDataInputStream::ReadEBOOL / ReadFloat (engine/CDataInputStream.h FABLE_RETAIL_CDATAINPUTSTREAM).
// Layout: PDB (Ego_r) CPSCSpline with the derived members 4 below the PDB (retail CPSCSplineBase is 0x34 bytes):
//   SplineCurrentPosition +0x34, ReverseSpline +0x38, SplineBounce +0x39, ScaleSplineSpeed +0x3A, SplineAnimSpeed +0x3C.
//   CPSCBase / CPSCSplineBase as in the landed CPSCSingleSprite::ReadBinary 0x00B068F0 model.
// Re-modelled 2026-11-03 (landing h): the row used to read through a local InputCalls stand-in derived from CDataInputStream.
#define FABLE_RETAIL_CDATAINPUTSTREAM
#include "engine/CDataInputStream.h"
#include "engine/CBaseClassNonCopyable.h"
namespace NParticleEngine {
class CParticleSystem;
template <class K, class V> class CParticleUpdateParamCallback : public CBaseClassNonCopyable { };
class CPSCBase : public CParticleUpdateParamCallback<unsigned long, unsigned long> {
public:
    CParticleSystem* ParticleSystem;                    // +0x04
    bool UpdateStarted : 1;                             // +0x08
    bool Initialised : 1;
    virtual void ReadBinary(CDataInputStream&) = 0;
};
class CPSCSplineBase : public CPSCBase {
public:
    unsigned char _spline[0x28];                        // +0x0c SplineAnimSpeed, Spline, SplineRandomOffsetData, SplinePosParamData (retail 0x34)
    virtual void ReadBinary(CDataInputStream& in);      // 0x00B22B40
};
class CPSCSpline : public CPSCSplineBase {
public:
    float SplineCurrentPosition;                        // +0x34
    bool  ReverseSpline;                                // +0x38
    bool  SplineBounce;                                 // +0x39
    bool  ScaleSplineSpeed;                             // +0x3a
    float SplineAnimSpeed;                              // +0x3c
    virtual void ReadBinary(CDataInputStream& in);
};
void CPSCSpline::ReadBinary(CDataInputStream& in)
{
    SplineBounce = in.ReadEBOOL();
    ScaleSplineSpeed = in.ReadEBOOL();
    SplineAnimSpeed = in.ReadFloat();
    CPSCSplineBase::ReadBinary(in);
}
}
