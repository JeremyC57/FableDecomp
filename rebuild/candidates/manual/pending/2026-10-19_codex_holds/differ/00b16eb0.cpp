// Grade: HELD(bytes match retail) -- test hand-transcribed with renamed types | Codex source 00/b1/NParticleEngineCPSCOrbit_CPSCOrbit_00b16eb0.cpp, flags , pass pattern 00b16eb0_TEST PASS
// NParticleEngine::CPSCOrbit::CPSCOrbit() -- initialize three orbit settings
struct CBase { virtual ~CBase(); CBase(); };
namespace NParticleEngine {
struct CParticleSystem;
struct CPSCBase : CBase { CParticleSystem* ParticleSystem; bool UpdateStarted:1; bool Initialised:1; CPSCBase(); };
struct COrbitSettings { long Type; bool Enabled; float Radius; float Expand; bool CycleFlag; float CycleTime; float SqueezeScale; float SqueezeAngle; float RotateSpeed; float RotateStart; float RotateSpeedRandom; float RotateStartRandom;
 COrbitSettings();
};
COrbitSettings::COrbitSettings() : Type(0), Enabled(false), Radius(1.0f), Expand(0), CycleFlag(false), CycleTime(5.0f), SqueezeScale(1.0f), SqueezeAngle(0), RotateSpeed(.25f), RotateStart(0), RotateSpeedRandom(0), RotateStartRandom(0) {}
struct CPSCOrbit : CPSCBase { unsigned long CentreParam; COrbitSettings Orbits[3]; unsigned char _pad[0xcc]; unsigned long State; CPSCOrbit(); };
CPSCOrbit::CPSCOrbit() { Orbits[0].Enabled=true; State=0; }
}