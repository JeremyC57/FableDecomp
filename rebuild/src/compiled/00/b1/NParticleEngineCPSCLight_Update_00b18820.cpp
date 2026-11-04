// NParticleEngine::CPSCLight::Update(CParticleSystem&, const CParticleUpdateParameters<unsigned long, unsigned long>&, bool& finished) -- tick the particle light's life timer: start it at minus the start time in ticks, count up through the timeline before the light starts, then hold it alive for its life seconds and (when it respawns) wait out the respawn delay before restarting; report alive into finished
// Leaf: ?Update@CPSCLight@NParticleEngine@@UAEXAAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@AA_N@Z
// Flags: none
// Retail-asm: 0x00B18852 `fistp dword ptr [esp+8]` -- `GetTickRate() * LightStartTime` stored to a frame float, reloaded and stored with a
//   bare dword fistp: GFFloatToLongNear (shared engine/GFFloatToLongNear.h). VC7.1 emits no bare dword fistp from C++ (NOTES.md "C++
//   attempts before helper"); the life-seconds compare is a plain `(long)` (__ftol2) in retail too. This row carries no asm of its own.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B18820, 304 bytes (+ int3 padding), thiscall `ret 0Ch`.
// Identity: FableWin.pdb ?Update@CPSCLight@NParticleEngine@@UAEXAAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@AA_N@Z.
// Callees: NParticleEngine::GetTickRate 0x00AEA9F0 (?GetTickRate@NParticleEngine@@YIJXZ), NParticleEngine::GetOneOverTickRate
//   0x00AEAA00 (FableWin ?GetOneOverTickRate@NParticleEngine@@YIMXZ, the PDB neighbour of Set/GetTickRate; the reloc bindings'
//   GetParticleUpdatePeriod / GetParticleFrameTime / GetParticleClock spellings are wrong, NAME_FIXES.tsv), __ftol2.
// Layout (Ego_r.pdb CPSCLight, the landed InterpolateTo 0x00B18C60 model): LightLifeTimer +0x0c, LightRespawnDelayTimer +0x10,
//   LightIsAlive +0x14, LightLifeSecs +0x1c, LightRespawnDelaySecs +0x20, LightStartTime +0x24, LightTimelineSecs +0x28; flag bytes
//   +0x48 (bit 1 LightUseLifeSecs, bit 3 LightRespawns) and +0x49 (bit 1 LightUseTimelineSecs, bit 2 StartedTimer).
// Shape: the body sits under `if (LightUseLifeSecs)` (the else stores LightIsAlive last); the tick rate is a long converted at each use;
//   the respawn test is `(float)delayTimer >= rate * delaySecs`; a failed respawn falls through to the shared `finished |= LightIsAlive`.
//   Grade: RELOCATION_MATCH (try f, 6 grades).
#include "engine/GFFloatToLongNear.h"
namespace NParticleEngine {
long __fastcall GetTickRate();                     // 0x00AEA9F0
float __fastcall GetOneOverTickRate();             // 0x00AEAA00
class CParticleSystem;
template <class A, class B> class CParticleUpdateParameters;
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
    virtual void Update(CParticleSystem& system, const CParticleUpdateParameters<unsigned long, unsigned long>& parameters, bool& finished);
};

void CPSCLight::Update(CParticleSystem&, const CParticleUpdateParameters<unsigned long, unsigned long>&, bool& finished)
{
    if (LightUseLifeSecs) {
        if (!StartedTimer) {
            LightLifeTimer = -GFFloatToLongNear((float)GetTickRate() * LightStartTime);
            StartedTimer = true;
        }
        float interval = GetOneOverTickRate();
        float now = (float)LightLifeTimer * interval + LightStartTime;
        if (LightLifeTimer < 0) {
            if (!LightUseTimelineSecs || now < LightTimelineSecs)
                ++LightLifeTimer;
            LightIsAlive = false;
            finished = true;
            return;
        }
        long tickRate = GetTickRate();
        if (LightLifeTimer != 0 && (LightLifeTimer > (long)((float)tickRate * LightLifeSecs) || !LightIsAlive)) {
            if (LightRespawns && (float)LightRespawnDelayTimer >= (float)tickRate * LightRespawnDelaySecs) {
                LightRespawnDelayTimer = 0;
                LightLifeTimer = 0;
                LightIsAlive = true;
            } else {
                ++LightRespawnDelayTimer;
                LightIsAlive = false;
            }
        } else if (!LightUseTimelineSecs || now < LightTimelineSecs) {
            ++LightLifeTimer;
            LightIsAlive = true;
        }
        finished |= LightIsAlive;
    } else
        LightIsAlive = true;
}
}