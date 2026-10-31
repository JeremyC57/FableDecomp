// NParticleEngine::CPSCRenderSprite::GetComponentCost(CParticleSystemCost& cost, const CParticleSystem& system, float particleCount, float) -- vtable slot 20: a sprite renderer adds its crossed-sprite polygons and the (system-scaled) sprite area per particle; a trail adds two polygons per particle and its scaled width x length
// Leaf: ?GetComponentCost@CPSCRenderSprite@NParticleEngine@@UAEXAAVCParticleSystemCost@2@ABVCParticleSystem@2@MM@Z
// Flags: /O2 /Oy /W3
// Retail-asm: 0x00AF9125 `fld dword ptr [esp+1Ch]` + 0x00AF9129 `fistp dword ptr [esp+18h]` -- the engine's inline-assembly float-to-long helper (the landed FloatToLong of CPSCEmitterGeneric::GetRandomRadius 0x00B102C0): the scaled size stored to the helper's by-value copy and reloaded for a bare dword fistp; VC7.1 `(long)x` calls __ftol2.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF90C0, 588 bytes, `ret 10h` (no catalog length: extent from the retail bytes, BOUNDS_FIX.tsv).
// Name: FableWin.pdb/Ego_r.pdb ?GetComponentCost@CPSCRenderSprite@NParticleEngine@@UAEXAAVCParticleSystemCost@2@ABVCParticleSystem@2@MM@Z
//   (the slot-20 spelling of the landed CPSCLight/CPSCOrbit/CPSCSpline GetComponentCost) + the manifest row (CPSCRenderSprite::GetComponentCost).
// Callee: CParticleSystem::ScaleParticleDimensions(float&) const @ 0x00AF8F80 (Ego_r.pdb QBEXAAM; NAME_FIXES.tsv: the reloc
//   binding and the landed definition are non-const). Its body sits in this TU, noinline and above the caller: that is what
//   gives retail's frame (two frame words, the quantiser temps in the dead cost/system argument slots); declared-only it
//   takes a third frame word. Land the GetComponentCost leaf only.
// Layout: Ego_r.pdb CPSCRenderSprite bitfields (dword +0x20: BlendMode..Flags..FadeInEndInteger; +0x24 TrailLengthInteger at
//   bit 10; +0x28 NoCrossedSprites bit 10, StartRenderSize bit 15; +0x2c EndRenderSize bit 7; +0x30 TrailWidth bit 21);
//   CParticleSystemCost as the landed CPSCLight::GetComponentCost.
// Constants: 4294967296.0f 0x0122DCB4, 1/2047 0x0129F65C, 20.0f 0x0122F160, 1.0f 0x0122DED8, 10.0f 0x01237F0C, 2.0f 0x0122DCDC,
//   1/1023 0x0129EF84, 0.001f 0x01266300, 0.5f 0x0122F59C (read from the binary).
namespace NParticleEngine {

class CParticleSystemCost { public:
    float PolyCount;                        // +0x00
    float PixelCount;                       // +0x04
    float LightCount;                       // +0x08
    float RenderCost;                       // +0x0c
    float SetupCost;                        // +0x10
    float LightCost;                        // +0x14
};

class C3DVector { public:
    float X, Y, Z;
};
class CParticleSystem { public:
    unsigned char m_pad00[0x50];
    bool          Initialised : 1;                   // +0x50 bit 0
    bool          Enabled : 1;
    bool          Dead : 1;
    bool          NormalisedTimeEnabled : 1;
    bool          UseAllAttractors : 1;
    bool          Visible : 1;
    bool          StayWithEmitter : 1;
    bool          ScaleParticles : 1;                // bit 7
    float         GlobalAlpha;                       // +0x54
    float         FadeAlpha;                         // +0x58
    float         NormalisedTime;                    // +0x5c
    C3DVector     Scale;                             // +0x60
    __declspec(noinline) void ScaleParticleDimensions(float& size) const   // 0x00AF8F80 (same-TU body, the landed one)
    {
        if (ScaleParticles) {
            float a = Scale.X > Scale.Y ? Scale.X : Scale.Y;
            size *= a > Scale.Z ? a : Scale.Z;
        }
    }
};

class CPSCBase { public:
    virtual ~CPSCBase();
    CParticleSystem* ParticleSystem;            // +0x04
    bool             UpdateStarted : 1;         // +0x08 bit 0
    bool             Initialised : 1;           // +0x08 bit 1
};

class CPSCRenderSprite : public CPSCBase { public:
    long          SpriteBankIndex;              // +0x0c
    long          TrailBankIndex;               // +0x10
    unsigned long StartColour;                  // +0x14 CRGBColour
    unsigned long MidColour;                    // +0x18
    unsigned long EndColour;                    // +0x1c
    unsigned long BlendMode : 3;                // +0x20
    unsigned long TrailBlendMode : 3;
    unsigned long BlendOp : 3;
    unsigned long TrailBlendOp : 3;
    unsigned long Flags : 10;
    unsigned long FadeInEndInteger : 10;
    unsigned long FadeOutBeginInteger : 10;     // +0x24
    unsigned long TrailLengthInteger : 12;
    unsigned long FlickerMinAlphaInteger : 10;
    unsigned long FlickerMinSizeInteger : 10;   // +0x28
    unsigned long NoCrossedSprites : 5;
    unsigned long StartRenderSize : 11;
    unsigned long AlphaFadeMinimum : 7;         // +0x2c
    unsigned long EndRenderSize : 11;
    unsigned long FlickerBias : 8;
    unsigned long AnimationTimeSecs : 14;       // +0x30
    unsigned long SizeFadeMinimum : 7;
    unsigned long TrailWidth : 10;
    unsigned long FlickerSpeed : 12;            // +0x34
    unsigned long UseStartColour : 1;
    unsigned long UseMidColour : 1;
    unsigned long UseEndColour : 1;
    virtual void GetComponentCost(CParticleSystemCost& cost, const CParticleSystem& system, float particleCount, float);
};

// the engine's inline-assembly float-to-long helper: VC7.1 expands a __forceinline function that contains __asm in place
static __forceinline long FloatToLong(float value)
{
    long result;
    __asm fld value
    __asm fistp result
    return result;
}
static __forceinline float RoundToDecimals(float value, long decimals)
{
    float scale = 1.0f;
    long i = decimals;
    do
        scale *= 10.0f;
    while (--i);
    float scaled = value * scale;
    return FloatToLong(scaled) / scale;
}

void CPSCRenderSprite::GetComponentCost(CParticleSystemCost& cost, const CParticleSystem& system, float particleCount, float)
{
    if (SpriteBankIndex) {
        float start = (float)StartRenderSize * (1.0f / 2047.0f);
        float startSize = RoundToDecimals(start * 20.0f, 2);
        float end = (float)EndRenderSize * (1.0f / 2047.0f);
        float endSize = RoundToDecimals(end * 20.0f, 2);
        float size = startSize > endSize ? startSize : endSize;
        system.ScaleParticleDimensions(size);
        float polys = (Flags & 0x40) ? (float)(NoCrossedSprites * 2) : 2.0f;
        polys *= particleCount;
        cost.PolyCount += polys;
        cost.RenderCost += size * size * particleCount + polys;
    }
    if (TrailBankIndex) {
        float unit = (float)TrailWidth * (1.0f / 1023.0f);
        float width = RoundToDecimals(unit * 10.0f, 2);
        float polys = particleCount + particleCount;
        float length = (float)TrailLengthInteger * 0.001f;
        if (length > 2.0f)
            length = 2.0f;
        length *= 0.5f;
        system.ScaleParticleDimensions(width);
        system.ScaleParticleDimensions(length);
        float area = length * width * particleCount;
        cost.PolyCount += polys;
        cost.PixelCount += area;
        cost.RenderCost += area + polys;
    }
}

} // namespace NParticleEngine