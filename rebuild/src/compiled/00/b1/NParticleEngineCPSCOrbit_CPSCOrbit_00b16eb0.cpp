// Grade: RELOCATION_MATCH (Fable landing 2026-10-19 after the Codex review hold: test hand-transcribed with renamed types)
// NParticleEngine::CPSCOrbit::CPSCOrbit() -- construct the particle-system component base, the three default orbit settings
//   (first one enabled), and zero the life timer
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B16EB0. Callee: NParticleEngine::CPSCBase::CPSCBase() 0x00AF2620 (manifest); vtable
//   0x012A0374 = NParticleEngine::CPSCOrbit.
// Layout: PDB (Ego_r) names/types, retail offsets equal the PDB: CPSCBase (vptr, ParticleSystem +0x04, flag bits +0x08),
//   CentreParam +0x0c, Orbits (3 x COrbitSettings, 0x30 each) +0x10, OrbitStates (3 x 0x44, opaque here) +0xa0,
//   LifeTimer +0x16c, LifeTimeSecs +0x170. Local model: no engine/ header exists for these classes.
namespace NParticleEngine {
struct CParticleSystem;
struct CPSCBase {
    virtual ~CPSCBase();
    CParticleSystem* ParticleSystem;                    // +0x04
    bool UpdateStarted : 1;                             // +0x08
    bool Initialised : 1;
    CPSCBase();                                         // 0x00AF2620
};
struct COrbitSettings {
    long Type; bool Enabled; float Radius; float Expand; bool CycleFlag; float CycleTime; float SqueezeScale; float SqueezeAngle;
    float RotateSpeed; float RotateStart; float RotateSpeedRandom; float RotateStartRandom;
    COrbitSettings();
};
inline COrbitSettings::COrbitSettings() : Type(0), Enabled(false), Radius(1.0f), Expand(0), CycleFlag(false), CycleTime(5.0f),
    SqueezeScale(1.0f), SqueezeAngle(0), RotateSpeed(.25f), RotateStart(0), RotateSpeedRandom(0), RotateStartRandom(0) {}
struct COrbitState { unsigned char _opaque[0x44]; };
struct CPSCOrbit : CPSCBase {
    unsigned long CentreParam;                          // +0x0c
    COrbitSettings Orbits[3];                           // +0x10
    COrbitState OrbitStates[3];                         // +0xa0
    long LifeTimer;                                     // +0x16c
    float LifeTimeSecs;                                 // +0x170
    CPSCOrbit();
};
CPSCOrbit::CPSCOrbit() { Orbits[0].Enabled = true; LifeTimer = 0; }
}