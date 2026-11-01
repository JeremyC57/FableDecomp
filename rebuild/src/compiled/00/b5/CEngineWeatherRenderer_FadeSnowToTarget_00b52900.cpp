// CEngineWeatherRenderer::FadeSnowToTarget(float target) -- step the current rain strength (sic, retail reads +0x5C8) 0.01 toward the target (snap to it when within 0.01) and apply it through StartSnowing
// Leaf: ?FadeSnowToTarget@CEngineWeatherRenderer@@AAEXM@Z
// Retail: Fable.exe (TLC, VC7.1) -- FadeRainToTarget @ 0x00B52870 and FadeSnowToTarget @ 0x00B52900, 132 bytes each, `ret 4`,
//   byte-identical except the tail-jump target (StartRaining 0x00B51EA0 / StartSnowing 0x00B51F00).
// Identity: Ego_r.pdb ?FadeRainToTarget@CEngineWeatherRenderer@@AAEXM@Z (donor 0x009F3770) and ?FadeSnowToTarget@..@AAEXM@Z
//   (donor 0x009F3810), private, donor order Rain then Snow = retail order. Each row is told apart by its tail jump: 0x00B52870
//   ends in StartRaining (stores +0x5C8 / flag +0x5D0, landed), 0x00B52900 in StartSnowing (stores +0x5CC / flag +0x5D1,
//   landed); the landed caller WorldFrameUpdate 0x00B52B30 calls 0x00B52870 in its rain block (0x00B52BA6 / 0x00B52BD9) and
//   0x00B52900 in its snow block (0x00B52C1D / 0x00B52C50). The catalog labelled both rows FadeRainToTarget (propagated).
// Retail quirk: BOTH bodies read Settings.CurrentRainStrength (+0x5C8, `mov eax,[ecx+5C8h]`); FadeSnowToTarget steps the snow
//   from the rain strength. Lionhead's copy-paste, kept as retail has it.
// Shape: `float strength = current; if ((float)fabs(strength - target) < 0.01f) strength = target; else if (strength > target)
//   strength -= 0.01f; else if (strength < target) strength += 0.01f; Start..(strength);` -- the `(float)` on fabs gives the
//   `fcomp dword` against the 0.01f pool (0x012A2418; without it VC compares a double); VC duplicates the tail call per arm.
// Layout: +0x5C8 is CEngineWeatherSettings::CurrentRainStrength of the Settings member at +0x114 (Ego_r.pdb: Settings +0x114,
//   CurrentRainStrength +0x4B4). engine/CEngineWeatherRenderer.h flattens Settings to bytes and carries the Ego_r layout after
//   it (OverridenRainStrength +0x12E8 where retail's StartRaining clamps against +0x12F8), so the class is modelled locally
//   (header debt in NOTES).
extern "C" double __cdecl fabs(double);
#pragma intrinsic(fabs)
class CEngineWeatherSettings { public:
    unsigned char _pad000[0x4b4];
    float         CurrentRainStrength;          // +0x4b4 (renderer +0x5c8)
    float         CurrentSnowStrength;          // +0x4b8 (renderer +0x5cc)
    bool          Raining;                      // +0x4bc
    bool          Snowing;                      // +0x4bd
    bool          Mist;                         // +0x4be
};
class CEngineWeatherRenderer { public:
    virtual void v00();
    unsigned char _pad004[0x110];
    CEngineWeatherSettings Settings;            // +0x114
    void StartRaining(float strength);          // 0x00B51EA0
    void StartSnowing(float strength);          // 0x00B51F00
private:
    void FadeRainToTarget(float target);
    void FadeSnowToTarget(float target);
};
void CEngineWeatherRenderer::FadeRainToTarget(float target)
{
    float strength = Settings.CurrentRainStrength;
    if ((float)fabs(strength - target) < 0.01f)
        strength = target;
    else if (strength > target)
        strength -= 0.01f;
    else if (strength < target)
        strength += 0.01f;
    StartRaining(strength);
}
void CEngineWeatherRenderer::FadeSnowToTarget(float target)
{
    float strength = Settings.CurrentRainStrength;
    if ((float)fabs(strength - target) < 0.01f)
        strength = target;
    else if (strength > target)
        strength -= 0.01f;
    else if (strength < target)
        strength += 0.01f;
    StartSnowing(strength);
}