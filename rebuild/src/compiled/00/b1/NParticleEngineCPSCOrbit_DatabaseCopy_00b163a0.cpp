// NParticleEngine::CPSCOrbit::DatabaseCopy(const CPSCBase& other) -- vtable slot 18: copy another orbit component's database settings (the centre parameter and the three orbit settings records)
// Leaf: ?DatabaseCopy@CPSCOrbit@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B163A0, 58 bytes (+6 int3 padding).
// Name: FableWin.pdb ?DatabaseCopy@CPSCOrbit@NParticleEngine@@UAEXABVCPSCBase@2@@Z; slot 18 of the vtable 0x012A0374
// (rtti-vtables.tsv; slot 18 of CPSCUpdateNormal is the landed DatabaseCopy 0x00B0B370).
// Callees: none.
// Layout: as the landed CPSCOrbit::WriteBinary 0x00B163E0: CentreParam +0x0C, COrbitSettings Orbits[3] +0x10
// (48 bytes each); the run-time OrbitStates (+0xA0) are not copied.
// Shape: the dword first (through ecx), then each 48-byte POD record by its implicit copy (`rep movsd`, 12
// dwords) with its own pair of `lea` -- three separate assignments, `this` kept in eax. `ret 4`.
struct CBase {
    virtual ~CBase();
};

namespace NParticleEngine {
class CParticleSystem;

struct COrbitSettings {                     // 48 bytes
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

class CPSCBase : public CBase { public:                   // vtable 0x0129F3F4
    CParticleSystem* ParticleSystem;        // +0x04
    bool             UpdateStarted : 1;     // +0x08 bit 0
    bool             Initialised : 1;       // +0x08 bit 1
};

class CPSCOrbit : public CPSCBase { public:               // vtable 0x012A0374
    unsigned long  CentreParam;             // +0x0c
    COrbitSettings Orbits[3];               // +0x10
    virtual void DatabaseCopy(const CPSCBase& other);
};

void CPSCOrbit::DatabaseCopy(const CPSCBase& other)
{
    const CPSCOrbit& source = static_cast<const CPSCOrbit&>(other);
    CentreParam = source.CentreParam;
    Orbits[0] = source.Orbits[0];
    Orbits[1] = source.Orbits[1];
    Orbits[2] = source.Orbits[2];
}
} // namespace NParticleEngine