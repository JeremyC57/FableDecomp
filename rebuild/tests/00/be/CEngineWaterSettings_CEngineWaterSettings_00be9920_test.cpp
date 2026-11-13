// Behaviour test for 0x00BE9920 CEngineWaterSettings::CEngineWaterSettings(): a host model of the constructor's logic (shore lookup
// mask matrices, zeroed shader matrices, switches, scalar defaults, cleared histories, the environment def's bump banks, the bound
// widths), checked against values computed independently below.
#include <cstdio>
#include <cmath>

struct V2 { float X, Y; };
struct M34 {
    float E11, E12, E13, E21, E22, E23, E31, E32, E33, E41, E42, E43;
    void Set(float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k, float l)
    { E11 = a; E12 = b; E13 = c; E21 = d; E22 = e; E23 = f; E31 = g; E32 = h; E33 = i; E41 = j; E42 = k; E43 = l; }
    void SetScale(float s) { E12 = E13 = E21 = E23 = E31 = E32 = E41 = E42 = E43 = 0.0f; E11 = E22 = E33 = s; }
};
struct EnvDef { float WaterLakeMinimumFlowSpeed, WaterLakeMaximumFlowSpeed; long WaterBumpMap, WaterBumpMap2; };

static float Mask(long step)
{
    double angle = step * (6.2831855f / 12.0);
    if (step < 6) return (float)(1.0 - std::cos(angle));
    return (float)(std::cos(angle) + 3.0);
}

struct Settings {
    bool ForceWaterSettings, EnableSea, EnableSeaForeground, EnableSeaMidground, EnableSeaBackground, EnableForeground,
         EnableBackground, EnableIceForeground, EnableIceBackground, EnableBumpMap, EnableSwell, EnableReflectionGeneration,
         EnableDrawReflection, EnableDrawBumpmap, EnableSpecular, EnableFresnel, EnableWaterOcclusion;
    float FresnelFactor, SwellFactor, ShoreSwellFactor, SwellOscilationSpeed, Colour[4], BumpmapSpan, SeaBumpmapSpan, LakeMin, LakeMax,
          FlowSpeedFactor, SoftEdgeMultFactor, EdgeWakeDepthClamp, RefOffset, BlendMult, BlendAdd;
    float SinShift, CosShift, ShoreSinShift, EdgeWakeOffset;
    float Histories[5][2], PCFrameOffset[3][2];
    V2 Shift[3][2], History[3][2][2];
    long BankIndex[2], PCFrameIndex[3];
    M34 Mask1, Mask2, Bounds, Other[4];

    void CalcReflectToRefractBlendFactors(float start, float end)
    {
        float scale = 1.0f / (end - start);
        BlendMult = -1.0f * scale;
        BlendAdd = 1.0f - start * scale;
    }
    explicit Settings(const EnvDef& def)
    {
        const long order[12] = { 0, 4, 8, 1, 5, 9, 2, 6, 10, 3, 7, 11 };
        float* m1 = &Mask1.E11;
        for (int i = 0; i < 12; ++i) m1[i] = Mask(order[i]);
        Mask2.Set(Mask(0), Mask(3), Mask(7), Mask(0), Mask(4), Mask(8), Mask(1), Mask(5), Mask(9), Mask(2), Mask(6), Mask(10));
        for (int i = 0; i < 4; ++i) Other[i].SetScale(0.0f);
        EnableSea = EnableSeaForeground = EnableSeaMidground = EnableSeaBackground = EnableForeground = EnableBackground = true;
        EnableIceForeground = EnableIceBackground = EnableSwell = EnableBumpMap = EnableReflectionGeneration = true;
        EnableDrawReflection = EnableDrawBumpmap = EnableSpecular = EnableFresnel = EnableWaterOcclusion = ForceWaterSettings = false;
        FresnelFactor = 2.0f;
        Colour[0] = Colour[1] = Colour[2] = Colour[3] = 1.0f;
        BumpmapSpan = 0.15f; SeaBumpmapSpan = 0.1f; SoftEdgeMultFactor = 3.0f; EdgeWakeDepthClamp = 2.0f;
        LakeMin = def.WaterLakeMinimumFlowSpeed; LakeMax = def.WaterLakeMaximumFlowSpeed;
        SwellOscilationSpeed = 0.05f; SwellFactor = 0.0625f; ShoreSwellFactor = 1.0f / 24.0f; FlowSpeedFactor = 0.25f; RefOffset = 0.01f;
        CalcReflectToRefractBlendFactors(0.0f, 15.0f);
        SinShift = CosShift = ShoreSinShift = EdgeWakeOffset = 0.0f;
        for (int i = 0; i < 3; ++i) PCFrameIndex[i] = 0;
        for (int i = 0; i < 2; ++i) for (int h = 0; h < 5; ++h) Histories[h][i] = 0.0f;
        for (int i = 0; i < 3; ++i) { PCFrameOffset[i][0] = PCFrameOffset[i][1] = 0.0f; }
        for (int t = 0; t < 3; ++t) for (int j = 0; j < 2; ++j) {
            Shift[t][j].X = Shift[t][j].Y = 0.0f;
            for (int k = 0; k < 2; ++k) History[t][j][k].X = History[t][j][k].Y = 0.0f;
        }
        BankIndex[0] = def.WaterBumpMap; BankIndex[1] = def.WaterBumpMap2;
        float bounds[12];
        const float* src = &Mask1.E11;
        for (int k = 0; k < 12; ++k) bounds[k] = src[(k % 4) * 3 + k / 4];      // pre-transposed (column) order
        for (long i = 0; i < 12; i++)
            for (long j = i - 1; j >= 0; j--)
                bounds[i] -= bounds[j];
        Bounds.Set(bounds[0], bounds[4], bounds[8], bounds[1], bounds[5], bounds[9], bounds[2], bounds[6], bounds[10],
                   bounds[3], bounds[7], bounds[11]);
    }
};

static bool Near(float a, float b) { return std::fabs(a - b) <= 1e-5f; }

int main()
{
    EnvDef def = { 0.25f, 4.0f, 17, 23 };
    Settings s(def);
    const float r3 = 0.8660254f;
    // the twelve phases 0, 30, ..., 330 degrees in column order
    const float phase[12] = { 0.0f, 1.0f - r3, 0.5f, 1.0f, 1.5f, 1.0f + r3, 2.0f, 3.0f - r3, 2.5f, 3.0f, 3.5f, 3.0f + r3 };
    const float* m1 = &s.Mask1.E11;
    const float* m2 = &s.Mask2.E11;
    const float* b = &s.Bounds.E11;
    bool ok = true;
    for (int r = 0; r < 4; ++r)
        for (int c = 0; c < 3; ++c) {
            int k = c * 4 + r;
            float prev = k > 0 ? phase[k - 1] : phase[0];
            ok = ok && Near(m1[r * 3 + c], phase[k]) && Near(m2[r * 3 + c], prev) && Near(b[r * 3 + c], phase[k] - prev);
        }
    ok = ok && s.Other[2].E11 == 0.0f && s.Other[3].E43 == 0.0f && s.Other[0].E22 == 0.0f;
    ok = ok && s.EnableSea && s.EnableSwell && s.EnableBumpMap && !s.EnableFresnel && !s.ForceWaterSettings;
    ok = ok && s.FresnelFactor == 2.0f && s.Colour[2] == 1.0f && s.LakeMin == 0.25f && s.LakeMax == 4.0f;
    ok = ok && Near(s.BlendMult, -1.0f / 15.0f) && s.BlendAdd == 1.0f && Near(s.ShoreSwellFactor, 0.0416667f);
    ok = ok && s.BankIndex[0] == 17 && s.BankIndex[1] == 23 && s.PCFrameIndex[2] == 0 && s.History[2][1][1].Y == 0.0f;
    if (!ok) { std::printf("00be9920_TEST FAIL\n"); return 1; }
    std::printf("00be9920_TEST PASS\n");
    return 0;
}