// NParticleEngine::CPSCSpline::GetUpdateParam(C3DVector* position, long, float timeScale, long, CParticleUpdateParameters& params, long) -- spline-follow component: make sure the per-update setup ran, advance along the spline by elapsed time x (speed / control-point count) x scale (backwards while bouncing in reverse), and fetch the point there
// Leaf: ?GetUpdateParam@CPSCSpline@NParticleEngine
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B1B030, length 130 bytes (padded to 144). Virtual: slot 1 of
// NParticleEngine::CPSCSpline's vtable 0x012A0934 (`ret 18h`, six stack arguments; the second, fourth and sixth are unused).
// Callees: virtual slot 8 (+0x20) on `this` (the update set-up, taking the parameters), the global particle frame time
//          getter @ 0x00AEAA00 (fld of the float @ 0x01399508; manifest "GetMaxDrawDistance"),
//          CPSCSplineBase::GetPointAtPos(CParticleUpdateParameters&, C3DVector*, float, float) @ 0x00B21940.
// Layout: retail CPSCBase (0xc): UpdateStarted bit 0 of +8; CPSCSplineBase (0x34): Spline control points vector +0x10/+0x14
// (12-byte C3DVector elements); CPSCSpline: SplineCurrentPosition +0x34, ReverseSpline +0x38, SplineBounce +0x39,
// SplineAnimSpeed +0x3c (PDB offsets are 4 higher).
// Shape: the frame time is a named float local (called before the count is formed, kept on the FPU); the control-point
// count is a signed size() converted through the dead params slot; the step is `time * (speed / count) * scale`; bounce-and-reverse subtracts, the else arm adds (`step + current`).
class C3DVector { public: float X, Y, Z; };
struct CParticleUpdateParameters;
float GetParticleFrameTime_00aeaa00();      // 0x00AEAA00

namespace NParticleEngine {
class CParticleSystem;
class CPSCBase { public:
    virtual ~CPSCBase();                                                    // slot 0
    virtual bool GetUpdateParam(C3DVector* position, long unused1, float timeScale, long unused2,
                                CParticleUpdateParameters& params, long unused3);   // slot 1
    virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void SetupUpdate(CParticleUpdateParameters& params);             // slot 8 (+0x20)
    CParticleSystem* ParticleSystem;        // +0x04
    bool             UpdateStarted : 1;     // +0x08 bit 0
    bool             Initialised : 1;       // +0x08 bit 1
};
class CPSCSplineBase : public CPSCBase { public:
    float      SplineAnimSpeedBase;         // +0x0c
    C3DVector* m_controlFirst;              // +0x10 Spline.ControlPoints
    C3DVector* m_controlLast;               // +0x14
    C3DVector* m_controlEnd;                // +0x18
    float      Tension;                     // +0x1c
    bool       EndPointsConnected;          // +0x20
    void*      SplineRandomOffsetData[2];   // +0x24
    void*      SplinePosParamData[2];       // +0x2c
    long GetControlPointCount() const { return m_controlLast - m_controlFirst; }
    void GetPointAtPos(CParticleUpdateParameters& params, C3DVector* position, float splinePos, float timeScale);   // 0x00B21940
};
class CPSCSpline : public CPSCSplineBase { public:        // vtable 0x012A0934
    float SplineCurrentPosition;            // +0x34
    bool  ReverseSpline;                    // +0x38
    bool  SplineBounce;                     // +0x39
    bool  ScaleSplineSpeed;                 // +0x3a
    float SplineAnimSpeed;                  // +0x3c
    virtual bool GetUpdateParam(C3DVector* position, long unused1, float timeScale, long unused2,
                                CParticleUpdateParameters& params, long unused3);
};

bool CPSCSpline::GetUpdateParam(C3DVector* position, long, float timeScale, long,
                                CParticleUpdateParameters& params, long)
{
    if (!UpdateStarted)
        SetupUpdate(params);
    float frameTime = GetParticleFrameTime_00aeaa00();
    float step = frameTime * (SplineAnimSpeed / GetControlPointCount()) * timeScale;
    float splinePos;
    if (SplineBounce && ReverseSpline)
        splinePos = SplineCurrentPosition - step;
    else
        splinePos = step + SplineCurrentPosition;
    GetPointAtPos(params, position, splinePos, timeScale);
    return true;
}
}