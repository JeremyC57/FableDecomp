// CPSCLight::Update(CParticleSystem*, CParticleUpdateParameters<unsigned long, unsigned long>*, bool* finished) -- tick the particle light's life timer: start it at minus the start-time in ticks, count up through the timeline before the light starts, then hold it alive for its life seconds and (when it respawns) wait out the respawn delay before restarting; report alive into *finished
// Leaf: ?Update@CPSCLight@@
// Retail-asm: 0x00B1884E `fld dword ptr [esp + 0xc]` ; 0x00B18852 `fistp dword ptr [esp + 8]` -- the engine's inline-assembly float-to-long helper (rasm1 family (a)): the value is stored to the helper's by-value parameter slot and reloaded for a bare dword fistp into a frame word; VC7.1 C++ `(long)x` calls __ftol2, /QIfist emits `fistp qword` and /arch:SSE2 cvttss2si (rasm1 probe family (a)). Everything around it is C++.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B18820; reconstructed from the parked reference body (rebuild/candidates/manual/pending), the C++ rounding stand-in replaced by the inline-assembly helper. The start tick is rounded by the helper; the life compare stays a __ftol2 cast.
struct CParticleSystem;
template <class A, class B> struct CParticleUpdateParameters;
namespace NParticleEngine {
    long GetParticleUpdateRate();                  // 0x00AEA9F0
    float GetParticleUpdateInterval();             // 0x00AEAA00
}
// the engine's inline-assembly float-to-long helper (VC7.1 expands the __forceinline body and its __asm in place)
__forceinline long FloatToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}


class CPSCLight {
public:
    virtual void v00();
    unsigned char _pad04[0x0c - 0x04];
    long  LightLifeTimer;                          // +0x0c
    long  LightRespawnDelayTimer;                  // +0x10
    bool  LightIsAlive;                            // +0x14
    unsigned long LightPositionParam;              // +0x18
    float LightLifeSecs;                           // +0x1c
    float LightRespawnDelaySecs;                   // +0x20
    float LightStartTime;                          // +0x24
    float LightTimelineSecs;                       // +0x28
    unsigned char _pad2c[0x48 - 0x2c];
    bool  LightWorldRadiusFadeEnable : 1;          // +0x48
    bool  LightUseLifeSecs : 1;
    bool  LightEnabled : 1;
    bool  LightRespawns : 1;
    bool  LightColourFadeEnable : 1;
    bool  LightUseStartColour : 1;
    bool  LightUseMidColour : 1;
    bool  LightUseEndColour : 1;
    bool  LightUseFadeColour : 1;                  // +0x49
    bool  LightUseTimelineSecs : 1;
    bool  StartedTimer : 1;
    void Update(CParticleSystem* system, CParticleUpdateParameters<unsigned long, unsigned long>* parameters, bool* finished);
};

void CPSCLight::Update(CParticleSystem*, CParticleUpdateParameters<unsigned long, unsigned long>*, bool* finished)
{
    if (!LightUseLifeSecs) {
        LightIsAlive = true;
        return;
    }
    if (!StartedTimer) {
        LightLifeTimer = -FloatToLong((float)NParticleEngine::GetParticleUpdateRate() * LightStartTime);
        StartedTimer = true;
    }
    float now = NParticleEngine::GetParticleUpdateInterval() * (float)LightLifeTimer + LightStartTime;
    if (LightLifeTimer < 0) {
        if (!LightUseTimelineSecs || now < LightTimelineSecs)
            ++LightLifeTimer;
        LightIsAlive = false;
        *finished = true;
        return;
    }
    float rate = (float)NParticleEngine::GetParticleUpdateRate();
    if (LightLifeTimer != 0 && (LightLifeTimer > (long)(rate * LightLifeSecs) || !LightIsAlive)) {
        if (LightRespawns && rate * LightRespawnDelaySecs <= (float)LightRespawnDelayTimer) {
            LightRespawnDelayTimer = 0;
            LightLifeTimer = 0;
            LightIsAlive = true;
        } else {
            ++LightRespawnDelayTimer;
            LightIsAlive = false;
            *finished |= LightIsAlive;
            return;
        }
    } else if (!LightUseTimelineSecs || now < LightTimelineSecs) {
        ++LightLifeTimer;
        LightIsAlive = true;
    }
    *finished |= LightIsAlive;
}
