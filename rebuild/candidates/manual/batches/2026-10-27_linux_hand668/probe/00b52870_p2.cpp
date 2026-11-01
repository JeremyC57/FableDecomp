// CEngineWeatherRenderer::FadeRainToTarget(float target) -- step the rain strength 0.01 toward the target
// Leaf: ?FadeRainToTarget@CEngineWeatherRenderer@@AAEXM@Z
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
