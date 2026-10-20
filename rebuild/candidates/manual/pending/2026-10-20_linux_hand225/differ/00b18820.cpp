// CPSCLight::Update(CParticleSystem*, CParticleUpdateParameters<unsigned long, unsigned long>*, bool* finished) -- tick the particle light's life timer: start it at minus the start-time in ticks, count up through the timeline before the light starts, then hold it alive for its life seconds and (when it respawns) wait out the respawn delay before restarting; report alive into *finished
// Grade: DIFFER(313v304) -- retail converts the start tick with a bare `fld ; fistp` (inline-asm float->long helper through a float temporary); ours is a C++ rounding (fcom/fadd/__ftol2); also the early return is laid out at the end in retail and the interval is the x87 left operand (fild ; fmul st(1))
// Leaf: ?Update@CPSCLight@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B18820, 304 bytes (+ int3 padding), `ret 0Ch`.
// Callees: NParticleEngine::GetParticleUpdateRate 0x00AEA9F0 (labels_chunk197 correction; manifest says
//   GFGetPreMainMemoryUsed), the update interval 0x00AEAA00 (manifest: GetMaxDrawDistance; returns a float), __ftol2.
// Layout (Ego_r.pdb CPSCLight): LightLifeTimer +0x0c, LightRespawnDelayTimer +0x10, LightIsAlive +0x14, LightLifeSecs
//   +0x1c, LightRespawnDelaySecs +0x20, LightStartTime +0x24, LightTimelineSecs +0x28; flag bytes +0x48 (bit 1
//   LightUseLifeSecs, bit 3 LightRespawns) and +0x49 (bit 1 LightUseTimelineSecs, bit 2 StartedTimer).
// Shape: retail's start-tick conversion is a bare `fld ; fistp` (the engine's inline-asm float->long helper, through a
//   float stack temporary); written here as a C++ conversion, so this row is an asm_bake candidate, not reconstruction.
struct CParticleSystem;
template <class A, class B> struct CParticleUpdateParameters;
namespace NParticleEngine {
    long GetParticleUpdateRate();                  // 0x00AEA9F0
    float GetParticleUpdateInterval();             // 0x00AEAA00
}
inline long FloatToLong(float value) { return (long)(value < 0.0f ? value - 0.5f : value + 0.5f); }

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
