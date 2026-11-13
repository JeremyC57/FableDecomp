// Behaviour test for 0x00BEA110 CEngineWaterSettings::UpdateConstants(const CEngineComponentRenderParams&): a host model of the
// row's logic (theme water values unless ForceWaterSettings, interpolated bump shifts / edge-wake offset / PC bump frame indices,
// swell sin / cos, and the four shader-variable matrices), checked against values computed independently below.
#include <cstdio>
#include <cmath>

struct V2 { float X, Y; };
struct M34 { float E[12]; void Set(const float* v) { for (int i = 0; i < 12; ++i) E[i] = v[i]; } };
struct Colour { float R, G, B, A; };

struct ThemeChannel { Colour Colours[19]; float Scalers[30]; };
struct EnvParams { ThemeChannel Channel0; };
struct GraphicInfo { unsigned short FrameCount; };
struct EnvDef { long WaterBumpMapPC, SeaBumpMapPC, IceBumpMapPC; };
struct World {
    const EnvParams* Env;
    const GraphicInfo* Graphics; unsigned long GraphicCount;
    const EnvDef* Def;
    long Width, Height;
    float ForegroundFadeStart, BackgroundX, BackgroundW;
    const GraphicInfo* PeekGraphic(unsigned long index) const { return index < GraphicCount ? &Graphics[index] : 0; }
};

// round to nearest, ties to even (the x87 default rounding of the bare fistp)
static long RoundNear(float x)
{
    double f = std::floor((double)x);
    double d = (double)x - f;
    long r = (long)f;
    if (d > 0.5 || (d == 0.5 && (r & 1))) ++r;
    return r;
}

struct Settings {
    bool ForceWaterSettings;
    Colour Col;
    float FlowSpeedFactor, SwellOscilationSpeed, SwellFactor, ShoreSwellFactor, RefOffset;
    float BumpmapSpan, SeaBumpmapSpan, SoftEdgeMultFactor, SoftEdgeAddFactor;
    float SinShift, CosShift, ShoreSinShift, EdgeWakeOffset;
    float SwellTrigIncrimentHistory[2], EdgeWakeOffsetHistory[2];
    float IceOffset[2], WaterOffset[2], SeaOffset[2];
    V2 Shift[3][2]; V2 History[3][2][2];
    float BlendMult, BlendAdd;
    long IceIndex, WaterIndex, SeaIndex;
    M34 Other, OtherSea, OtherBackground, OtherSeaBackground;

    void CalcReflectToRefractBlendFactors(float start, float end)
    {
        float scale = 1.0f / (end - start);
        BlendMult = -1.0f * scale;
        BlendAdd = 1.0f - start * scale;
    }
    static long FrameIndex(const GraphicInfo* g, const float* offset, float t)
    {
        if (g && g->FrameCount > 0)
            return (long)((unsigned long)RoundNear((offset[0] - offset[1]) * t + offset[1]) % (unsigned long)g->FrameCount);
        return 0;
    }
    void UpdateConstants(const World& w, float t)
    {
        if (!ForceWaterSettings) {
            const ThemeChannel& c = w.Env->Channel0;
            Col = c.Colours[14];
            CalcReflectToRefractBlendFactors(c.Scalers[23], c.Scalers[24]);
            FlowSpeedFactor = c.Scalers[25];
            SwellOscilationSpeed = c.Scalers[26];
            SwellFactor = c.Scalers[27] * 0.0001f;
            ShoreSwellFactor = c.Scalers[28];
            RefOffset = c.Scalers[29];
        }
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 2; ++j) {
                Shift[i][j].X = (History[i][j][0].X - History[i][j][1].X) * t + History[i][j][1].X;
                Shift[i][j].Y = (History[i][j][0].Y - History[i][j][1].Y) * t + History[i][j][1].Y;
            }
        EdgeWakeOffset = (EdgeWakeOffsetHistory[0] - EdgeWakeOffsetHistory[1]) * t + EdgeWakeOffsetHistory[1];
        const GraphicInfo* ice = w.PeekGraphic(w.Def->IceBumpMapPC);
        const GraphicInfo* water = w.PeekGraphic(w.Def->WaterBumpMapPC);
        const GraphicInfo* sea = w.PeekGraphic(w.Def->SeaBumpMapPC);
        IceIndex = FrameIndex(ice, IceOffset, t);
        WaterIndex = FrameIndex(water, WaterOffset, t);
        SeaIndex = FrameIndex(sea, SeaOffset, t);
        float swell = (((SwellTrigIncrimentHistory[0] - SwellTrigIncrimentHistory[1]) * t) + SwellTrigIncrimentHistory[1]) * 0.15915494f;
        double angle = swell * (double)6.2831855f;
        SinShift = (float)std::sin(angle);
        CosShift = (float)std::cos(angle);
        ShoreSinShift = SinShift - 1.0f;
        float inv_width = 1.0f / w.Width, inv_height = 1.0f / w.Height;
        float fade = w.ForegroundFadeStart - 22.627417f, inv_fade = 1.0f / fade;
        float a[12] = { BumpmapSpan, inv_width, BlendMult, ShoreSinShift, inv_height, BlendAdd, SinShift, fade, SoftEdgeMultFactor,
                        CosShift, inv_fade, SoftEdgeAddFactor };
        Other.Set(a);
        a[0] = SeaBumpmapSpan; OtherSea.Set(a);
        float b[12] = { BumpmapSpan, inv_width, 0.0f, w.BackgroundX, inv_height, 0.0f, w.BackgroundW, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f };
        OtherBackground.Set(b);
        b[0] = SeaBumpmapSpan; OtherSeaBackground.Set(b);
    }
};

static bool Near(float a, float b) { return std::fabs(a - b) <= 1e-4f * (1.0f + std::fabs(b)); }

int main()
{
    EnvParams env;
    for (int i = 0; i < 19; ++i) { env.Channel0.Colours[i].R = (float)i; env.Channel0.Colours[i].G = i + 0.25f;
        env.Channel0.Colours[i].B = i + 0.5f; env.Channel0.Colours[i].A = 1.0f; }
    for (int i = 0; i < 30; ++i) env.Channel0.Scalers[i] = 10.0f + i;
    GraphicInfo graphics[3] = { { 8 }, { 0 }, { 5 } };
    EnvDef def = { 0, 1, 7 };                  // water -> 8 frames, sea -> 0 frames, ice -> out of range
    World w = { &env, graphics, 3, &def, 800, 600, 100.0f, 0.75f, 2.5f };

    Settings s;
    s.ForceWaterSettings = false;
    s.BumpmapSpan = 2.0f; s.SeaBumpmapSpan = 4.0f; s.SoftEdgeMultFactor = 3.0f; s.SoftEdgeAddFactor = 0.5f;
    s.SwellTrigIncrimentHistory[0] = 3.0f; s.SwellTrigIncrimentHistory[1] = 1.0f;
    s.EdgeWakeOffsetHistory[0] = 6.0f; s.EdgeWakeOffsetHistory[1] = 2.0f;
    s.IceOffset[0] = 3.0f; s.IceOffset[1] = 1.0f;
    s.WaterOffset[0] = 30.0f; s.WaterOffset[1] = 10.0f;   // lerp 20.0 -> 20 % 8 = 4
    s.SeaOffset[0] = 9.0f; s.SeaOffset[1] = 1.0f;
    for (int i = 0; i < 3; ++i) for (int j = 0; j < 2; ++j) {
        s.Shift[i][j].X = s.Shift[i][j].Y = -7.0f;
        s.History[i][j][0].X = 4.0f + i; s.History[i][j][0].Y = 8.0f + j;
        s.History[i][j][1].X = 2.0f;     s.History[i][j][1].Y = 0.0f;
    }
    s.IceIndex = s.WaterIndex = s.SeaIndex = 99;
    s.UpdateConstants(w, 0.5f);

    bool ok = true;
    ok = ok && s.Col.R == 14.0f && s.Col.G == 14.25f;
    // blend factors from scalers 23 (33) / 24 (34): scale 1, mult -1, add 1 - 33 = -32
    ok = ok && Near(s.BlendMult, -1.0f) && Near(s.BlendAdd, -32.0f);
    ok = ok && s.FlowSpeedFactor == 35.0f && s.SwellOscilationSpeed == 36.0f && Near(s.SwellFactor, 0.0037f)
            && s.ShoreSwellFactor == 38.0f && s.RefOffset == 39.0f;
    ok = ok && Near(s.Shift[1][0].X, 3.5f) && Near(s.Shift[0][1].Y, 4.5f) && s.Shift[2][0].X == -7.0f;   // [2] untouched
    ok = ok && Near(s.EdgeWakeOffset, 4.0f);
    ok = ok && s.IceIndex == 0 && s.WaterIndex == 4 && s.SeaIndex == 0;
    // swell lerp 2.0 rad -> turns 2/(2pi) -> sin(2), cos(2)
    ok = ok && Near(s.SinShift, (float)std::sin(2.0)) && Near(s.CosShift, (float)std::cos(2.0)) && Near(s.ShoreSinShift, (float)std::sin(2.0) - 1.0f);
    ok = ok && Near(s.Other.E[1], 1.0f / 800.0f) && Near(s.Other.E[4], 1.0f / 600.0f) && Near(s.Other.E[7], 100.0f - 22.627417f)
            && Near(s.Other.E[10], 1.0f / (100.0f - 22.627417f)) && s.Other.E[0] == 2.0f && s.OtherSea.E[0] == 4.0f
            && s.Other.E[8] == 3.0f && s.OtherSea.E[11] == 0.5f;
    ok = ok && s.OtherBackground.E[3] == 0.75f && s.OtherBackground.E[6] == 2.5f && s.OtherBackground.E[9] == 1.0f
            && s.OtherBackground.E[2] == 0.0f && s.OtherSeaBackground.E[0] == 4.0f && s.OtherSeaBackground.E[11] == 0.0f;

    // forced settings keep the theme values out
    Settings f = s;
    f.ForceWaterSettings = true; f.FlowSpeedFactor = -1.0f; f.Col.R = -2.0f;
    f.UpdateConstants(w, 0.5f);
    ok = ok && f.FlowSpeedFactor == -1.0f && f.Col.R == -2.0f;
    // rounding: 2.5 -> 2 (ties to even), 3.5 -> 4
    ok = ok && RoundNear(2.5f) == 2 && RoundNear(3.5f) == 4 && RoundNear(-1.5f) == -2;

    if (!ok) { std::printf("00bea110_TEST FAIL\n"); return 1; }
    std::printf("00bea110_TEST PASS\n");
    return 0;
}
