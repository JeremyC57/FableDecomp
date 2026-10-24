// NParticleEngine::CPSCSpline::InterpolateTo(const CPSCBase& other, float t) -- vtable slot 14: blend towards another spline component (the spline base, a lerp of the anim speed, the bounce and speed-scaling flags taken from the target past the halfway point)
// Leaf: ?InterpolateTo@CPSCSpline@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B1AF50, 111 bytes (+1 int3 padding).
// Name: FableWin.pdb ?InterpolateTo@CPSCSpline@NParticleEngine@@UAEXABVCPSCBase@2@M@Z; slot 14 of the vtable 0x012A0934
// (rtti-vtables.tsv).
// Callees: CPSCSplineBase::InterpolateTo(const CPSCBase*, float) @ 0x00B22310 (landed; non-virtual, pointer argument).
// Constants: 1.0f @ 0x0122DED8 and 0.5f @ 0x0122F59C (compiler literals).
// Layout: as the landed CPSCSpline::WriteBinary 0x00B1AFF0: SplineBounce +0x39, ScaleSplineSpeed +0x3A,
// SplineAnimSpeed +0x3C.
// Shape: `(1 - t) * own + t * other` (fld 1 ; fsub t ; fmul [esi+3Ch] ; fld t ; fmul [edi+3Ch] ; faddp);
// each flag is `t > 0.5f ? other : own` (`test ah,41h ; jne` to the own arm), the second one's store
// tail-duplicated into both exits. `ret 8`.
namespace NParticleEngine {

struct CParticleSystem;

struct CPSCBase {
    virtual void v00();
    CParticleSystem* ParticleSystem;        // +0x04
    bool UpdateStarted : 1;                 // +0x08
    bool Initialised : 1;
};

struct CPSCSplineBase : CPSCBase {
    unsigned char _body[0x34 - 0x0c];       // +0x0c SplineAnimSpeed, Spline, the two counted data pointers
    void InterpolateTo(const CPSCBase* other, float t);   // 0x00B22310
};

struct CPSCSpline : CPSCSplineBase {        // vtable 0x012A0934
    float SplineCurrentPosition;            // +0x34
    bool  ReverseSpline;                    // +0x38
    bool  SplineBounce;                     // +0x39
    bool  ScaleSplineSpeed;                 // +0x3a
    float SplineAnimSpeed;                  // +0x3c
    virtual void InterpolateTo(const CPSCBase& other, float t);
};

void CPSCSpline::InterpolateTo(const CPSCBase& other, float t)
{
    const CPSCSpline& source = static_cast<const CPSCSpline&>(other);
    CPSCSplineBase::InterpolateTo(&other, t);
    SplineAnimSpeed = (1.0f - t) * SplineAnimSpeed + t * source.SplineAnimSpeed;
    SplineBounce = t > 0.5f ? source.SplineBounce : SplineBounce;
    ScaleSplineSpeed = t > 0.5f ? source.ScaleSplineSpeed : ScaleSplineSpeed;
}

} // namespace NParticleEngine