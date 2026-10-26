// CEngineSimulationManager::PausedWorldFrameUpdate() -- while the world is paused: refresh the wind vector from the current environment's wind (direction * strength) and count one more pending paused step
// Leaf: PausedWorldFrameUpdate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B80840, length 156 bytes (virtual, `?PausedWorldFrameUpdate@CEngineSimulationManager@@UAEXXZ`).
// Callees: virtual slot 1 of the environment provider at +0x198 of the engine state @ 0x01436E8C (returns the current
// environment), virtual slot 1 of that (returns the wind record: direction C3DVector at +0x14, strength float at +0x20);
// the chain is walked twice (once per field).
// Layout: the PDB layout (engine/CEngineSimulationManager.h) has three 12-byte std::lists before the noise table; the
// retail STLport lists are 4 bytes wide, which moves WindVector from +0x106c to +0x1050 and PausedStepsPending from
// +0x108c to +0x1070 (both 0x1c lower), so `this` is modelled locally with the PDB names.
// Shape: the direction is copied into a C3DVector local through a memberwise user copy constructor (the two first
// words are read at +0x14/+0x18 before `add eax,14h`; the implicit copy forms the address first), `direction * strength`
// builds a by-value C3DVector temporary (user constructor) in the second frame vector that is then copied into the
// member through ecx/edx; the strength stays on the FPU across the three products; `++PausedStepsPending` last.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
};

struct SWindRecord {
    unsigned char _pad[0x14];
    C3DVector     Direction;                       // +0x14
    float         Strength;                        // +0x20
};

class CEnvironment { public:
    virtual void v00();
    virtual SWindRecord* GetWind();                // slot 1
};

struct CEnvironmentProvider {
    virtual void v00();
    virtual CEnvironment* GetCurrentEnvironment(); // slot 1
};

struct SEngineState_01436e8c {
    unsigned char         _pad[0x198];
    CEnvironmentProvider* m_pEnvironmentProvider;  // +0x198
};
extern SEngineState_01436e8c* g_pEngineState_01436e8c;   // 0x01436E8C

class CEngineSimulationManager { public:
    unsigned char _pad[0x104c];                     // vptr at +0x00
    C3DVector     WindVector;                      // +0x1050
    unsigned char _pad105c[0x14];
    unsigned long PausedStepsPending;              // +0x1070
    virtual void PausedWorldFrameUpdate();
};

void CEngineSimulationManager::PausedWorldFrameUpdate()
{
    C3DVector direction = g_pEngineState_01436e8c->m_pEnvironmentProvider->GetCurrentEnvironment()->GetWind()->Direction;
    WindVector = direction * g_pEngineState_01436e8c->m_pEnvironmentProvider->GetCurrentEnvironment()->GetWind()->Strength;
    ++PausedStepsPending;
}