// NParticleEngine::CPSCOrbit::StartUpdate(const CParticleUpdateParameters<unsigned long, unsigned long>& params) -- per-frame orbit update: on the first frame seed the three orbit states and run four warm-up steps, then advance each orbit around the component's current centre
// Leaf: StartUpdate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B16D50, length 337 bytes (padded to 352), `ret 4`.
// Callees: particle frame-time getter @ 0x00AEAA00 (fld of the float @ 0x01399508; manifest "GetMaxDrawDistance"),
//          particle random-seed getter @ 0x00AEAAD0 (returns &seed @ 0x013D2E10; manifest "AddChildPrimitive" is wrong),
//          COrbitState::Initialise(const COrbitSettings&, unsigned long*) @ 0x00B161A0 (landed),
//          CPSCBase::GetCurrentPositionLocal(params, const unsigned long& param, float, bool, unsigned long) const @ 0x00AF2780
//          (by-value C3DVector, hidden result pointer; manifest "CParticleUpdateParameters_TransformPosition"),
//          COrbitState::Update(const COrbitSettings&, CMatrix3x4&, float) @ 0x00B16410.
// Identity: Ego_r.pdb `?StartUpdate@CPSCOrbit@NParticleEngine@@UAEXABV?$CParticleUpdateParameters@KK@2@@Z`.
// Layout: landed CPSCOrbit model (0x00B16EB0 ctor): CPSCBase (vptr, ParticleSystem +4, UpdateStarted bit 0 of +8),
//   CentreParam +0xc, Orbits[3] (0x30) +0x10, OrbitStates[3] (0x44) +0xa0, LifeTimer +0x16c, LifeTimeSecs +0x170.
// Shape: the warm-up count is 1 or 4 (an unsigned loop VC turns into a down-counter in the dead `params` slot); the transform
//   handed to every Update is rebuilt each pass as an identity rotation (chained zero/one assignments, stored from E32/E33
//   downwards) with the centre as translation.
class C3DVector { public:
    float X, Y, Z;
};

class CMatrix3x4 {
public:
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    C3DVector Pos;
};


float GetParticleFrameTime();                   // 0x00AEAA00
unsigned long* GetParticleRandomSeed();         // 0x00AEAAD0

namespace NParticleEngine {

template <class A, class B> class CParticleUpdateParameters;   // NParticleEngine (Ego_r / FableWin)

class CParticleSystem;

struct COrbitSettings {                         // 48 bytes
    long  Type;
    bool  Enabled;
    float Radius;
    float Expand;
    bool  CycleFlag;
    float CycleTime;
    float SqueezeScale;
    float SqueezeAngle;
    float RotateSpeed;
    float RotateStart;
    float RotateSpeedRandom;
    float RotateStartRandom;
};

struct COrbitState {                            // 0x44 bytes
    unsigned char _body[0x44];
    void Initialise(const COrbitSettings& settings, unsigned long* seed);               // 0x00B161A0
    void Update(const COrbitSettings& settings, CMatrix3x4& transform, float time);     // 0x00B16410
};

class CPSCBase { public:
    virtual ~CPSCBase();
    CParticleSystem* ParticleSystem;            // +0x04
    bool             UpdateStarted : 1;         // +0x08 bit 0
    bool             Initialised : 1;           // +0x08 bit 1
    C3DVector GetCurrentPositionLocal(const CParticleUpdateParameters<unsigned long, unsigned long>& params,
                                 const unsigned long& param, float scale, bool local, unsigned long flags) const;  // 0x00AF2780 (Ego_r / FableWin)
};

class CPSCOrbit : public CPSCBase { public:                   // vtable 0x012A0374
    unsigned long  CentreParam;                 // +0x0c
    COrbitSettings Orbits[3];                   // +0x10
    COrbitState    OrbitStates[3];              // +0xa0
    long           LifeTimer;                   // +0x16c
    float          LifeTimeSecs;                // +0x170
    virtual void StartUpdate(const CParticleUpdateParameters<unsigned long, unsigned long>& params);
};

void CPSCOrbit::StartUpdate(const CParticleUpdateParameters<unsigned long, unsigned long>& params)
{
    UpdateStarted = true;
    float frameTime = GetParticleFrameTime();
    unsigned long updates = 1;
    if (LifeTimer == 0) {
        for (int i = 0; i < 3; ++i)
            OrbitStates[i].Initialise(Orbits[i], GetParticleRandomSeed());
        updates = 4;
        LifeTimeSecs = -frameTime;
    }
    C3DVector centre = GetCurrentPositionLocal(params, CentreParam, 1.0f, false, 0);
    for (unsigned long n = 0; n < updates; ++n) {
        CMatrix3x4 transform;
        transform.E12 = transform.E13 = transform.E21 = transform.E23 = transform.E31 = transform.E32 = 0.0f;
        transform.E11 = transform.E22 = transform.E33 = 1.0f;
        transform.Pos = centre;
        for (int i = 0; i < 3; ++i)
            OrbitStates[i].Update(Orbits[i], transform, LifeTimeSecs);
        LifeTimeSecs = frameTime + LifeTimeSecs;
    }
    ++LifeTimer;
}

} // namespace NParticleEngine