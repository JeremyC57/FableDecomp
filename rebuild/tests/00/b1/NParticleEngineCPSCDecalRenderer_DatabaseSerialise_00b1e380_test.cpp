// Behaviour test for NParticleEngine::CPSCDecalRenderer::DatabaseSerialise (0x00B1E380): the row's body verbatim over a recording /
// replaying persist context (per-type name -> value maps; saving records, loading replays or falls back to the default), a symbol map
// and the renderer's real packed layout (the bit-field words +0x20..+0x30 and the flag bytes +0x34 / +0x35 from engine/CPSCDecalRenderer.h):
// save records the unpacked values and the sprite name; load writes every field into its bit-field without disturbing neighbours,
// re-packs the floats, resolves the sprite name (0 when unknown) and rebuilds Flags from the legacy booleans when SpriteFlags is absent.
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <map>
#include <string>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("check failed line %d\n", __LINE__); ++g_fail; } } while (0)
class CCharString { public: std::string S; CCharString() {} CCharString(const char* s) : S(s) {} bool operator<(const CCharString& o) const { return S < o.S; } };
static inline CCharString EmptyCCharString() { CCharString empty; return empty; }
class CRGBColour { public: unsigned char B, G, R, A;
    CRGBColour(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0, unsigned char a = 0) : B(b), G(g), R(r), A(a) {}
    bool operator==(const CRGBColour& o) const { return B == o.B && G == o.G && R == o.R && A == o.A; } };
static long GFFloatToLongNear(float x) { return (long)floor(x + 0.5f); }
static float GFRoundFloatNear(float value, long places) { float scale = 1.0f; for (long i = places; i != 0; --i) scale *= 10.0f; return GFFloatToLongNear(value * scale) / scale; }
static unsigned long GFPackFloat(float value, unsigned long bits, float lo, float hi, unsigned long places)
{ float n = (GFRoundFloatNear(value, places) - lo) / (hi - lo); return (unsigned long)GFFloatToLongNear(n * ((float)(long)(1 << bits) - 1.0f)); }
static float GFUnpackFloat(unsigned long packed, unsigned long bits, float lo, float hi, unsigned long places)
{ float n = packed / ((float)(long)(1 << bits) - 1.0f); return GFRoundFloatNear(n * (hi - lo) + lo, places); }
class CPersistContext { public:
    bool Saving;
    std::map<std::string, bool> Bools; std::map<std::string, long> Longs; std::map<std::string, float> Floats;
    std::map<std::string, CCharString> Strings; std::map<std::string, CRGBColour> Colours; std::map<std::string, int> Seen;
    CPersistContext(bool saving) : Saving(saving) {}
    bool IsSaving() const { return Saving; }
    template<class T> void Do(std::map<std::string, T>& m, const char* name, T& v, const T& def)
    { ++Seen[name]; if (Saving) m[name] = v; else { typename std::map<std::string, T>::iterator it = m.find(name); v = it != m.end() ? it->second : def; } }
    void Transfer(const char* n, bool& v, const bool& d) { Do(Bools, n, v, d); }
    void Transfer(const char* n, long& v, const long& d) { Do(Longs, n, v, d); }
    void Transfer(const char* n, float& v, const float& d) { Do(Floats, n, v, d); }
    void Transfer(const char* n, CCharString& v, const CCharString& d) { Do(Strings, n, v, d); }
    void Transfer(const char* n, CRGBColour& v, const CRGBColour& d) { Do(Colours, n, v, d); } };
class CSymbolMap { public:
    class CSymbolInfo { public: CCharString EntryName; long Value; };
    CSymbolInfo Info[2];
    CSymbolMap() { Info[0].EntryName = "DECAL_A"; Info[0].Value = 5; Info[1].EntryName = "DECAL_B"; Info[1].Value = 9; }
    const CSymbolInfo* FindFirstSymbolWithVal(const char* type, long v) const
    { if (strcmp(type, "EEngineGraphic")) return 0; for (int i = 0; i < 2; ++i) if (Info[i].Value == v) return &Info[i]; return 0; }
    bool SymbolExists(const CCharString& s) const { for (int i = 0; i < 2; ++i) if (Info[i].EntryName.S == s.S) return true; return false; }
    long GetSymbolSLONG(const CCharString& s) const { for (int i = 0; i < 2; ++i) if (Info[i].EntryName.S == s.S) return Info[i].Value; return -99; } };
class CEmitterDatabaseSerialiseInfo { public: CSymbolMap GraphicSymbols; };
namespace NParticleEngine {
class CParticleEmitterManager { public: long GetDefaultBlendMode() const { return 2; } long GetDefaultBlendOp() const { return 0; } };
static CParticleEmitterManager Manager;
static CParticleEmitterManager& GetParticleEmitterManager() { return Manager; }
class CPSCDecalRenderer { public:
    void* VTable; void* System; long Base8;
    float         DecalLifeSecs;                // +0x0c
    long          DecalBankIndex;               // +0x10
    CRGBColour    StartColour;                  // +0x14
    CRGBColour    MidColour;                    // +0x18
    CRGBColour    EndColour;                    // +0x1c
    unsigned long BlendMode : 3;                // +0x20
    unsigned long BlendOp : 3;
    unsigned long Flags : 10;
    unsigned long FadeInEndInteger : 10;
    unsigned long FadeOutBeginInteger : 10;     // +0x24
    unsigned long StartRenderSize : 11;
    unsigned long AlphaFadeMinimum : 7;
    unsigned long EndRenderSize : 11;           // +0x28
    unsigned long AnimationTimeSecs : 14;
    unsigned long SizeFadeMinimum : 7;
    unsigned long UseStartColour : 1;           // +0x2c
    unsigned long UseMidColour : 1;
    unsigned long UseEndColour : 1;
    unsigned long FlickerBias : 8;
    unsigned long FlickerMinAlphaInteger : 10;
    unsigned long FlickerMinSizeInteger : 10;
    unsigned long FlickerSpeed : 12;            // +0x30
    unsigned char AlphaFadeEnable : 1;          // +0x34
    unsigned char SizeFadeEnable : 1;
    unsigned char ForceAnimationTime : 1;
    unsigned char FlickerEnable : 1;
    unsigned char PoolingEnable : 1;
    unsigned char PoolIncreaseAlphaEnable : 1;
    unsigned char PoolIncreaseSizeEnable : 1;
    unsigned char PoolIncreaseLifeEnable : 1;
    unsigned char PoolIncreaseFrameEnable : 1;  // +0x35
    float         MaxPoolSize;                  // +0x38
    float         MaxPoolAlpha;                 // +0x3c
    float         MaxPoolLife;                  // +0x40
    float         PoolFrameIncreaseRate;        // +0x44
    float         StencilCubeWidth;             // +0x48
    void DatabaseSerialise(CPersistContext& context, const CEmitterDatabaseSerialiseInfo& info); };
// ---- the row's body, verbatim ----
void CPSCDecalRenderer::DatabaseSerialise(CPersistContext& context, const CEmitterDatabaseSerialiseInfo& info)
{
    context.Transfer("DecalLifeSecs", DecalLifeSecs, 0.0f);
    if (context.IsSaving()) {
        CCharString spriteName;
        const CSymbolMap::CSymbolInfo* symbol = info.GraphicSymbols.FindFirstSymbolWithVal("EEngineGraphic", DecalBankIndex);
        if (symbol)
            spriteName = symbol->EntryName;
        context.Transfer("SpriteName", spriteName, EmptyCCharString());
        {
            long spriteFlags = Flags;
            context.Transfer("SpriteFlags", spriteFlags, 0L);
            Flags = spriteFlags;
        }
    } else {
        CCharString spriteName;
        context.Transfer("SpriteName", spriteName, EmptyCCharString());
        if (info.GraphicSymbols.SymbolExists(spriteName))
            DecalBankIndex = info.GraphicSymbols.GetSymbolSLONG(spriteName);
        else
            DecalBankIndex = 0;
        long spriteFlags = Flags;
        context.Transfer("SpriteFlags", spriteFlags, -1L);
        if (spriteFlags == -1) {
            bool legacyFlag;
            context.Transfer("FaceMe2D", legacyFlag, false);
            Flags = 0;
            context.Transfer("FaceMe3D", legacyFlag, false);
            Flags = Flags | (legacyFlag ? 1 : 0);
            context.Transfer("CrossedSprites", legacyFlag, false);
            Flags = Flags | (legacyFlag ? 0x40 : 0);
            context.Transfer("RotateAroundCentre", legacyFlag, false);
            Flags = Flags | (legacyFlag ? 0x20 : 0);
            context.Transfer("SelfIlluminating", legacyFlag, false);
            Flags = Flags | (legacyFlag ? 0 : 0x80);
            long spriteAlignment = 0;
            context.Transfer("SpriteAlignment", spriteAlignment, 0L);
            Flags |= spriteAlignment;
        } else {
            Flags = spriteFlags;
        }
    }
    {
        bool forceAnimationTime = ForceAnimationTime;
        context.Transfer("ForceAnimationTime", forceAnimationTime, false);
        ForceAnimationTime = forceAnimationTime;
    }
    {
        bool useStartColour = UseStartColour;
        context.Transfer("UseStartColour", useStartColour, true);
        UseStartColour = useStartColour;
    }
    {
        bool useMidColour = UseMidColour;
        context.Transfer("UseMidColour", useMidColour, false);
        UseMidColour = useMidColour;
    }
    {
        bool useEndColour = UseEndColour;
        context.Transfer("UseEndColour", useEndColour, false);
        UseEndColour = useEndColour;
    }
    context.Transfer("StartColour", StartColour, CRGBColour(255, 255, 255, 255));
    context.Transfer("MidColour", MidColour, CRGBColour(255, 255, 255, 255));
    context.Transfer("EndColour", EndColour, CRGBColour(255, 255, 255, 255));
    {
        long blendMode = BlendMode;
        context.Transfer("BlendMode", blendMode, GetParticleEmitterManager().GetDefaultBlendMode());
        BlendMode = blendMode;
    }
    {
        long blendOp = BlendOp;
        context.Transfer("BlendOp", blendOp, GetParticleEmitterManager().GetDefaultBlendOp());
        BlendOp = blendOp;
    }
    {
        long fadeInEndInteger = FadeInEndInteger;
        context.Transfer("FadeInEndInteger", fadeInEndInteger, 0L);
        FadeInEndInteger = fadeInEndInteger;
    }
    {
        long fadeOutBeginInteger = FadeOutBeginInteger;
        context.Transfer("FadeOutBeginInteger", fadeOutBeginInteger, 0L);
        FadeOutBeginInteger = fadeOutBeginInteger;
    }
    {
        bool alphaFadeEnable = AlphaFadeEnable;
        context.Transfer("AlphaFadeEnable", alphaFadeEnable, false);
        AlphaFadeEnable = alphaFadeEnable;
    }
    {
        bool sizeFadeEnable = SizeFadeEnable;
        context.Transfer("SizeFadeEnable", sizeFadeEnable, false);
        SizeFadeEnable = sizeFadeEnable;
    }
    {
        float alphaFadeMinimum = GFRoundFloatNear(AlphaFadeMinimum / 127.0f, 2);
        context.Transfer("AlphaFadeMinimum", alphaFadeMinimum, 1.0f);
        AlphaFadeMinimum = GFPackFloat(alphaFadeMinimum, 7, 0.0f, 1.0f, 2);
    }
    {
        float sizeFadeMinimum = GFRoundFloatNear(SizeFadeMinimum / 127.0f, 2);
        context.Transfer("SizeFadeMinimum", sizeFadeMinimum, 1.0f);
        SizeFadeMinimum = GFPackFloat(sizeFadeMinimum, 7, 0.0f, 1.0f, 2);
    }
    {
        float animationTimeSecs = GFUnpackFloat(AnimationTimeSecs, 14, 0.1f, 100.0f, 2);
        context.Transfer("AnimationTimeSecs", animationTimeSecs, 1.0f);
        AnimationTimeSecs = GFPackFloat(animationTimeSecs, 14, 0.1f, 100.0f, 2);
    }
    {
        float startRenderSize = GFUnpackFloat(StartRenderSize, 11, 0.0f, 20.0f, 2);
        context.Transfer("StartRenderSize", startRenderSize, 1.0f);
        StartRenderSize = GFPackFloat(startRenderSize, 11, 0.0f, 20.0f, 2);
    }
    {
        float endRenderSize = GFUnpackFloat(EndRenderSize, 11, 0.0f, 20.0f, 2);
        context.Transfer("EndRenderSize", endRenderSize, 1.0f);
        EndRenderSize = GFPackFloat(endRenderSize, 11, 0.0f, 20.0f, 2);
    }
    {
        bool flickerEnable = FlickerEnable;
        context.Transfer("FlickerEnable", flickerEnable, false);
        FlickerEnable = flickerEnable;
    }
    {
        long flickerMinAlphaInteger = FlickerMinAlphaInteger;
        context.Transfer("FlickerMinAlphaInteger", flickerMinAlphaInteger, 0L);
        FlickerMinAlphaInteger = flickerMinAlphaInteger;
    }
    {
        long flickerMinSizeInteger = FlickerMinSizeInteger;
        context.Transfer("FlickerMinSizeInteger", flickerMinSizeInteger, 0L);
        FlickerMinSizeInteger = flickerMinSizeInteger;
    }
    {
        float flickerSpeed = GFUnpackFloat(FlickerSpeed, 12, 0.0f, 30.0f, 2);
        context.Transfer("FlickerSpeed", flickerSpeed, 15.0f);
        FlickerSpeed = GFPackFloat(flickerSpeed, 12, 0.0f, 30.0f, 2);
    }
    {
        float flickerBias = GFUnpackFloat(FlickerBias, 8, -1.0f, 1.0f, 2);
        context.Transfer("FlickerBias", flickerBias, 0.0f);
        FlickerBias = GFPackFloat(flickerBias, 8, -1.0f, 1.0f, 2);
    }
    {
        bool poolingEnable = PoolingEnable;
        context.Transfer("PoolingEnable", poolingEnable, false);
        PoolingEnable = poolingEnable;
    }
    {
        bool poolIncreaseAlphaEnable = PoolIncreaseAlphaEnable;
        context.Transfer("PoolIncreaseAlphaEnable", poolIncreaseAlphaEnable, false);
        PoolIncreaseAlphaEnable = poolIncreaseAlphaEnable;
    }
    {
        bool poolIncreaseSizeEnable = PoolIncreaseSizeEnable;
        context.Transfer("PoolIncreaseSizeEnable", poolIncreaseSizeEnable, false);
        PoolIncreaseSizeEnable = poolIncreaseSizeEnable;
    }
    {
        bool poolIncreaseLifeEnable = PoolIncreaseLifeEnable;
        context.Transfer("PoolIncreaseLifeEnable", poolIncreaseLifeEnable, false);
        PoolIncreaseLifeEnable = poolIncreaseLifeEnable;
    }
    context.Transfer("MaxPoolSize", MaxPoolSize, 0.0f);
    context.Transfer("MaxPoolAlpha", MaxPoolAlpha, 0.0f);
    context.Transfer("MaxPoolLife", MaxPoolLife, 0.0f);
    {
        bool poolIncreaseFrameEnable = PoolIncreaseFrameEnable;
        context.Transfer("PoolIncreaseFrameEnable", poolIncreaseFrameEnable, false);
        PoolIncreaseFrameEnable = poolIncreaseFrameEnable;
    }
    context.Transfer("PoolFrameIncreaseRate", PoolFrameIncreaseRate, 0.0f);
    context.Transfer("StencilCubeWidth", StencilCubeWidth, 0.0f);
}
}
using namespace NParticleEngine;
static void Fill(CPSCDecalRenderer& d, unsigned char pattern) { memset(&d, pattern, sizeof(d)); }
int main()
{
    CEmitterDatabaseSerialiseInfo info;
    {   // save
        CPSCDecalRenderer d; Fill(d, 0); d.DecalLifeSecs = 2.5f; d.DecalBankIndex = 9; d.Flags = 0x61; d.AlphaFadeMinimum = 127;
        d.AnimationTimeSecs = 16383; d.FlickerBias = 255; d.PoolIncreaseSizeEnable = 1; d.UseMidColour = 1; d.BlendMode = 3; d.FadeOutBeginInteger = 700;
        d.MaxPoolLife = 4.0f; d.StartColour = CRGBColour(1, 2, 3, 4);
        CPersistContext c(true); d.DatabaseSerialise(c, info);
        CHECK(c.Floats["DecalLifeSecs"] == 2.5f && c.Strings["SpriteName"].S == "DECAL_B" && c.Longs["SpriteFlags"] == 0x61);
        CHECK(c.Floats["AlphaFadeMinimum"] == 1.0f && c.Floats["AnimationTimeSecs"] == 100.0f && c.Floats["FlickerBias"] == 1.0f);
        CHECK(c.Bools["PoolIncreaseSizeEnable"] && !c.Bools["PoolIncreaseLifeEnable"] && c.Bools["UseMidColour"] && !c.Bools["UseStartColour"]);
        CHECK(c.Longs["BlendMode"] == 3 && c.Longs["FadeOutBeginInteger"] == 700 && c.Floats["MaxPoolLife"] == 4.0f);
        CHECK(c.Colours["StartColour"] == CRGBColour(1, 2, 3, 4) && c.Seen.count("FaceMe2D") == 0 && c.Seen["StencilCubeWidth"] == 1);
        CHECK(memcmp(&d, &d, sizeof(d)) == 0 && d.DecalBankIndex == 9 && d.Flags == 0x61 && d.PoolIncreaseSizeEnable == 1 && d.AlphaFadeMinimum == 127); }
    for (int pattern = 0; pattern < 2; ++pattern) {   // load: every field replayed into stale bits
        CPSCDecalRenderer d; Fill(d, pattern ? 0xFF : 0x00);
        CPersistContext c(false);
        c.Strings["SpriteName"] = "DECAL_A"; c.Longs["SpriteFlags"] = 0x123; c.Bools["PoolIncreaseSizeEnable"] = true;
        c.Bools["PoolIncreaseLifeEnable"] = false; c.Bools["PoolIncreaseFrameEnable"] = true; c.Bools["ForceAnimationTime"] = false;
        c.Bools["UseStartColour"] = true; c.Bools["UseMidColour"] = false; c.Bools["UseEndColour"] = true; c.Bools["FlickerEnable"] = false;
        c.Bools["AlphaFadeEnable"] = true; c.Bools["SizeFadeEnable"] = false; c.Bools["PoolingEnable"] = true; c.Bools["PoolIncreaseAlphaEnable"] = false;
        c.Longs["BlendMode"] = 5; c.Longs["BlendOp"] = 2; c.Longs["FadeInEndInteger"] = 1023; c.Longs["FadeOutBeginInteger"] = 1;
        c.Longs["FlickerMinAlphaInteger"] = 512; c.Longs["FlickerMinSizeInteger"] = 3; c.Floats["AlphaFadeMinimum"] = 0.5f;
        c.Floats["SizeFadeMinimum"] = 0.0f; c.Floats["AnimationTimeSecs"] = 0.1f; c.Floats["StartRenderSize"] = 20.0f; c.Floats["EndRenderSize"] = 10.0f;
        c.Floats["FlickerSpeed"] = 30.0f; c.Floats["FlickerBias"] = -1.0f; c.Floats["MaxPoolSize"] = 1.5f; c.Floats["StencilCubeWidth"] = 7.0f;
        d.DatabaseSerialise(c, info);
        CHECK(d.DecalBankIndex == 5 && d.Flags == 0x123 && d.BlendMode == 5 && d.BlendOp == 2 && d.FadeInEndInteger == 1023 && d.FadeOutBeginInteger == 1);
        CHECK(d.PoolIncreaseSizeEnable == 1 && d.PoolIncreaseLifeEnable == 0 && d.PoolIncreaseFrameEnable == 1 && d.ForceAnimationTime == 0);
        CHECK(d.UseStartColour == 1 && d.UseMidColour == 0 && d.UseEndColour == 1 && d.FlickerEnable == 0 && d.AlphaFadeEnable == 1 && d.SizeFadeEnable == 0);
        CHECK(d.PoolingEnable == 1 && d.PoolIncreaseAlphaEnable == 0 && d.FlickerMinAlphaInteger == 512 && d.FlickerMinSizeInteger == 3);
        CHECK(d.AlphaFadeMinimum == 64 && d.SizeFadeMinimum == 0 && d.AnimationTimeSecs == 0 && d.StartRenderSize == 2047 && d.EndRenderSize == 1024);
        CHECK(d.FlickerSpeed == 4095 && d.FlickerBias == 0 && d.MaxPoolSize == 1.5f && d.StencilCubeWidth == 7.0f && d.DecalLifeSecs == 0.0f);
        CHECK(d.StartColour == CRGBColour(255, 255, 255, 255) && d.MaxPoolAlpha == 0.0f && d.PoolFrameIncreaseRate == 0.0f); }
    {   // load: unknown sprite, legacy flags (SpriteFlags absent)
        CPSCDecalRenderer d; Fill(d, 0); d.DecalBankIndex = 9;
        CPersistContext c(false); c.Strings["SpriteName"] = "NOPE"; c.Bools["FaceMe3D"] = true; c.Bools["CrossedSprites"] = true;
        c.Bools["SelfIlluminating"] = false; c.Longs["SpriteAlignment"] = 0x100;
        d.DatabaseSerialise(c, info);
        CHECK(d.DecalBankIndex == 0 && d.Flags == (0x1 | 0x40 | 0x80 | 0x100) && c.Seen["FaceMe2D"] == 1 && c.Seen["RotateAroundCentre"] == 1);
        CHECK(d.UseStartColour == 1 && d.BlendMode == 2 && d.BlendOp == 0 && d.FlickerSpeed == 2048 && d.AlphaFadeMinimum == 127); }
    printf(g_fail ? "00b1e380_TEST FAIL\n" : "00b1e380_TEST PASS\n");
    return g_fail ? 1 : 0;
}