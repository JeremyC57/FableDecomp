// NParticleEngine::CPSCLight::EnterTimelineEnd() -- the light's timeline has ended: stop timing the light against the timeline
// Leaf: ?EnterTimelineEnd@CPSCLight@NParticleEngine@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B1AE60, 5 bytes. Slot 25 (+0x64) of the CPSCLight vtable 0x012A0624 (absolute ref
//   0x012A0688), the slot CPSCEmitterGeneric fills with its EnterTimelineEnd (0x00B15AF0); Ego_r.pdb has
//   `?EnterTimelineEnd@CPSCLight@NParticleEngine@@UAEXXZ`.
// Layout: Ego_r.pdb CPSCLight: bool bit fields from +0x48 -- LightWorldRadiusFadeEnable, LightUseLifeSecs, LightEnabled,
//   LightRespawns, LightColourFadeEnable, LightUseStartColour, LightUseMidColour, LightUseEndColour (+0x48), then
//   LightUseFadeColour, LightUseTimelineSecs (bit 1), StartedTimer (+0x49); clearing LightUseTimelineSecs is
//   `and byte ptr [ecx+49h],0FDh`.
namespace NParticleEngine {

class CPSCBase {
public:
    void*         ParticleSystem;                          // +0x04
    unsigned char Flags;                                   // +0x08 (UpdateStarted / Initialised bits)
    virtual void EnterTimelineEnd();                       // slot 25 (+0x64)
};

class CPSCLight : public CPSCBase {
public:
    long  LightLifeTimer;                                  // +0x0C
    long  LightRespawnDelayTimer;                          // +0x10
    bool  LightIsAlive;                                    // +0x14
    unsigned long LightPositionParam;                      // +0x18
    float LightLifeSecs;                                   // +0x1C
    float LightRespawnDelaySecs;                           // +0x20
    float LightStartTime;                                  // +0x24
    float LightTimelineSecs;                               // +0x28
    float LightStartRenderWorldRadius;                     // +0x2C
    float LightEndRenderWorldRadius;                       // +0x30
    long  LightAttenuationFactor;                          // +0x34
    long  LightFadeInEndInteger;                           // +0x38
    long  LightFadeOutBeginInteger;                        // +0x3C
    float LightWorldRadiusFadeMinimum;                     // +0x40
    unsigned long LightColourFadeMinimum;                  // +0x44 CRGBColour
    bool  LightWorldRadiusFadeEnable : 1;                  // +0x48
    bool  LightUseLifeSecs : 1;
    bool  LightEnabled : 1;
    bool  LightRespawns : 1;
    bool  LightColourFadeEnable : 1;
    bool  LightUseStartColour : 1;
    bool  LightUseMidColour : 1;
    bool  LightUseEndColour : 1;
    bool  LightUseFadeColour : 1;                          // +0x49 bit 0
    bool  LightUseTimelineSecs : 1;                        // +0x49 bit 1
    bool  StartedTimer : 1;                                // +0x49 bit 2
    virtual void EnterTimelineEnd();
};

void CPSCLight::EnterTimelineEnd()
{
    LightUseTimelineSecs = false;
}

}   // namespace NParticleEngine