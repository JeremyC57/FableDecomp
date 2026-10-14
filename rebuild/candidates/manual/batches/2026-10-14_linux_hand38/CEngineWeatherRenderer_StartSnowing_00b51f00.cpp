// CEngineWeatherRenderer::StartSnowing(float strength) -- override the snow: a strength that rounds to no particles (strength * 1024 truncated <= 0) clears the override; otherwise the strength itself is clamped to MaximumWeatherParticlePercentage, stored, and on the first activation the box position matrix is rebuilt and the override flag raised
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B51F00, length 96 bytes.
// Callees: __ftol2 @ 0x00BFEA70 (the `(long)` cast; the 1024.0f constant lives in the pool @ 0x01230010), CEngineWeatherRenderer::BuildBoxPositionMatrix
// @ 0x00B51AD0. Shape: `fld arg ; fmul const` is `1024.0f * strength`; the clamp is a float `<` compare (`fcomp ; fnstsw ; test ah,5 ; jp`)
// choosing between the argument and the member (`a < b ? a : b`); `ret 4`: the `[esp+4]` read before `push esi` and the `[esp+8]` reads after it
// are the SAME single argument, the old flag byte is read before the float store. The rain twin
// @ 0x00B51EA0 (manifest "StartRaining") uses the previous byte/float pair, so this body is the snow variant. The engine header puts
// OverridenSnowStrength/SnowOverriden at +0x12f0/+0x12f4 (Ego PDB); retail has the float at +0x5cc and the flag at +0x5d1 while
// MaximumWeatherParticlePercentage stays at +0x12f8, so the retail layout is modelled locally.
struct CEngineWeatherRenderer {
    unsigned char _pad0[0x5c8];
    float         OverridenRainStrength;           // +0x5c8
    float         OverridenSnowStrength;           // +0x5cc
    bool          RainOverriden;                   // +0x5d0
    bool          SnowOverriden;                   // +0x5d1
    unsigned char _pad1[0x12f8 - 0x5d2];
    float         MaximumWeatherParticlePercentage;// +0x12f8
    void BuildBoxPositionMatrix();                 // 0x00B51AD0
    void StartSnowing(float strength);
};

void CEngineWeatherRenderer::StartSnowing(float strength)
{
    if ((long)(1024.0f * strength) <= 0) {
        SnowOverriden = false;
        return;
    }
    OverridenSnowStrength = strength < MaximumWeatherParticlePercentage ? strength : MaximumWeatherParticlePercentage;
    if (!SnowOverriden) {
        BuildBoxPositionMatrix();
        SnowOverriden = true;
    }
}
