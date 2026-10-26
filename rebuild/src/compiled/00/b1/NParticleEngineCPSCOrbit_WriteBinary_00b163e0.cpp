// NParticleEngine::CPSCOrbit::WriteBinary(CDataOutputStream*) -- writes the centre-position parameter id, then the three orbit settings records as one raw 0x90-byte block
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B163E0, length 40 bytes.
// Callees: CDataOutputStream::WriteULONG @ 0x00993EA0, CDataOutputStream virtual slot 4 (+0x10) WriteData(const void*, unsigned long).
// Identity: slot 16 of NParticleEngine::CPSCOrbit's vtable 0x012A0374 (rtti-vtables.tsv).
// Layout (Ego_r.pdb, CPSCOrbit 0x174): CentreParam +0xc, COrbitSettings Orbits[3] +0x10 (48 bytes each:
// Type, Enabled, Radius, Expand, CycleFlag, CycleTime, the squeeze and rotate floats), OrbitStates +0xa0.
// Shape: the member is loaded before the stream argument; the array's address is formed with
// `add esi,10h` because `this` is dead after it.
struct CBase {
    virtual ~CBase();
};

class CDataOutputStream { public:
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void WriteData(const void* data, unsigned long size);   // slot 4 (+0x10)
public:
    void WriteULONG(unsigned long);   // 0x00993EA0
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
    void WriteBinary(CDataOutputStream* out);
};

void CPSCOrbit::WriteBinary(CDataOutputStream* out)
{
    out->WriteULONG(CentreParam);
    out->WriteData(Orbits, sizeof(Orbits));
}
}