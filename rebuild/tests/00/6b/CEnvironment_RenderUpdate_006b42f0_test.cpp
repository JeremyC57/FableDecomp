// Behaviour test for 0x006B42F0 CEnvironment::RenderUpdate(float frame_fraction): a standalone copy of the row's logic over a
// small model (6 blended themes per tick state, 19 colours, 30 scalers, 3 texture sets of two arrays each), with stand-ins for the
// out-of-line callees (Update, Blend, AccurateNormalise, the three spell-effect getters, the engine's SetScreenColourFilter).
#include <cstdio>
#include <cmath>

struct Colour { float R, G, B, A; };
struct Vec3 {
    float X, Y, Z;
    Vec3 operator*(float s) const { Vec3 r; r.X = X * s; r.Y = Y * s; r.Z = Z * s; return r; }
    Vec3 operator+(const Vec3& o) const { Vec3 r; r.X = X + o.X; r.Y = Y + o.Y; r.Z = Z + o.Z; return r; }
    void AccurateNormalise() { float m = (float)std::sqrt(X * X + Y * Y + Z * Z); if (m > 0.0f) { X /= m; Y /= m; Z /= m; } }
};
struct TextureSet { long Textures[2]; float Blends[2]; };
struct Theme { float TimeOfDay; Colour Colours[19]; float Scalers[30]; TextureSet TextureSets[3];
    // stand-in for 0x00887B70 Blend: out = this * (1 - t) + other * t, componentwise
    void Blend(Theme& out, const Theme& other, float t) const {
        int i;
        out.TimeOfDay = TimeOfDay * (1.0f - t) + other.TimeOfDay * t;
        for (i = 0; i < 19; ++i) {
            out.Colours[i].R = Colours[i].R * (1.0f - t) + other.Colours[i].R * t;
            out.Colours[i].G = Colours[i].G; out.Colours[i].B = other.Colours[i].B; out.Colours[i].A = 1.0f;
        }
        for (i = 0; i < 30; ++i) out.Scalers[i] = Scalers[i] * (1.0f - t) + other.Scalers[i] * t;
        for (i = 0; i < 3; ++i) { out.TextureSets[i] = t < 0.5f ? TextureSets[i] : other.TextureSets[i]; }
    }
};
struct Channel { Colour Colours[19]; float Scalers[30]; TextureSet TextureSets[3]; };
struct TickState { Theme BlendedTheme[6]; float Time, WindSpeed; Vec3 WindDirection; float ThemeWindSpeed;
    Vec3 ThemeWindDirection, SunVector, MoonVector, DiffuseVector, ShadowVector; };
struct RadialBlur { float V[20]; };
struct Displacement { Colour MonochromeEffectColour; };
struct ScreenFilter { float Alpha, Saturation, Contrast, Brightness; Colour Tint; };
struct FilterSettings { float Alpha, Saturation, Contrast, Brightness; Colour Tint; };
struct Params { float Time; Vec3 WindDirection; float WindSpeed; Vec3 ThemeWindDirection; float ThemeWindSpeed;
    Vec3 SunVector, MoonVector, DiffuseVector, ShadowVector; Channel ThemeChannels[6]; long ChannelCount;
    RadialBlur Blur; Displacement Disp; };

static int g_update_calls = 0;
static float g_last_fraction[3];
static FilterSettings g_filter;
static int g_filter_calls = 0;

struct SpellInterface {
    RadialBlur GetRadialBlur(float t) { g_last_fraction[0] = t; RadialBlur r; for (int i = 0; i < 20; ++i) r.V[i] = t + (float)i; return r; }
    Displacement GetDisplacement(float t) { g_last_fraction[1] = t; Displacement d; d.MonochromeEffectColour.R = t; d.MonochromeEffectColour.G = 2.0f;
        d.MonochromeEffectColour.B = 3.0f; d.MonochromeEffectColour.A = 4.0f; return d; }
    ScreenFilter GetScreenFilter(float t) { g_last_fraction[2] = t; ScreenFilter f; f.Alpha = t; f.Saturation = 0.5f; f.Contrast = 0.25f;
        f.Brightness = 0.125f; f.Tint.R = 1.0f; f.Tint.G = 0.75f; f.Tint.B = 0.5f; f.Tint.A = 0.25f; return f; }
};
static void SetScreenColourFilter(const FilterSettings& s) { g_filter = s; ++g_filter_calls; }

struct Env {
    bool PassedFirstUpdate;
    TickState GameTickState, PrevGameTickState;
    Params InterpolatedEnvironmentParams;
    SpellInterface* SpellEffectInterface;
    void Update(float) { ++g_update_calls; PassedFirstUpdate = true; }
    void RenderUpdate(float frame_fraction) {
        if (!PassedFirstUpdate) { Update(0.0f); }
        if (frame_fraction < 0.0f) frame_fraction = 0.0f;           // GFLimit(frame_fraction, 0.0f, 1.0f)
        else if (frame_fraction > 1.0f) frame_fraction = 1.0f;
        float one_minus = 1.0f - frame_fraction;
        Params& p = InterpolatedEnvironmentParams;
        p.Time = PrevGameTickState.Time * one_minus + GameTickState.Time * frame_fraction;
        p.ChannelCount = 6;                                          // ThemeChannels.resize(6)
        static Theme interpolated_theme;
        for (long channel = 0; channel < 6; ++channel) {
            PrevGameTickState.BlendedTheme[channel].Blend(interpolated_theme, GameTickState.BlendedTheme[channel], frame_fraction);
            long i;
            for (i = 0; i < 19; ++i) p.ThemeChannels[channel].Colours[i] = interpolated_theme.Colours[i];
            for (i = 0; i < 30; ++i) p.ThemeChannels[channel].Scalers[i] = interpolated_theme.Scalers[i];
            for (i = 0; i < 3; ++i) p.ThemeChannels[channel].TextureSets[i] = interpolated_theme.TextureSets[i];
        }
        p.WindDirection = PrevGameTickState.WindDirection * one_minus + GameTickState.WindDirection * frame_fraction;
        p.WindSpeed = PrevGameTickState.WindSpeed * one_minus + GameTickState.WindSpeed * frame_fraction;
        p.ThemeWindDirection = PrevGameTickState.ThemeWindDirection * one_minus + GameTickState.ThemeWindDirection * frame_fraction;
        p.ThemeWindSpeed = PrevGameTickState.ThemeWindSpeed * one_minus + GameTickState.ThemeWindSpeed * frame_fraction;
        p.SunVector = PrevGameTickState.SunVector * one_minus + GameTickState.SunVector * frame_fraction;
        p.MoonVector = PrevGameTickState.MoonVector * one_minus + GameTickState.MoonVector * frame_fraction;
        p.DiffuseVector = PrevGameTickState.DiffuseVector * one_minus + GameTickState.DiffuseVector * frame_fraction;
        p.SunVector.AccurateNormalise();
        p.MoonVector.AccurateNormalise();
        p.DiffuseVector.AccurateNormalise();
        p.Blur = SpellEffectInterface->GetRadialBlur(frame_fraction);
        p.Disp = SpellEffectInterface->GetDisplacement(frame_fraction);
        ScreenFilter sf = SpellEffectInterface->GetScreenFilter(frame_fraction);
        FilterSettings fs;
        fs.Alpha = sf.Alpha; fs.Saturation = sf.Saturation; fs.Contrast = sf.Contrast; fs.Brightness = sf.Brightness; fs.Tint = sf.Tint;
        SetScreenColourFilter(fs);
    }
};

static bool near_(float a, float b) { return std::fabs(a - b) < 1e-4f; }
static Vec3 V(float x, float y, float z) { Vec3 v; v.X = x; v.Y = y; v.Z = z; return v; }

int main()
{
    static Env env;
    static SpellInterface spell;
    int fail = 0;
    env.PassedFirstUpdate = false;
    env.SpellEffectInterface = &spell;
    for (int k = 0; k < 6; ++k) {
        for (int i = 0; i < 30; ++i) { env.PrevGameTickState.BlendedTheme[k].Scalers[i] = (float)(k * 100 + i); env.GameTickState.BlendedTheme[k].Scalers[i] = (float)(k * 100 + i + 10); }
        for (int i = 0; i < 19; ++i) { env.PrevGameTickState.BlendedTheme[k].Colours[i].R = 0.0f; env.GameTickState.BlendedTheme[k].Colours[i].R = 1.0f;
            env.PrevGameTickState.BlendedTheme[k].Colours[i].G = (float)k; env.GameTickState.BlendedTheme[k].Colours[i].B = (float)i; }
        for (int i = 0; i < 3; ++i) { env.PrevGameTickState.BlendedTheme[k].TextureSets[i].Textures[0] = 1; env.GameTickState.BlendedTheme[k].TextureSets[i].Textures[0] = 2; }
    }
    env.PrevGameTickState.Time = 10.0f; env.GameTickState.Time = 20.0f;
    env.PrevGameTickState.WindSpeed = 1.0f; env.GameTickState.WindSpeed = 3.0f;
    env.PrevGameTickState.ThemeWindSpeed = 2.0f; env.GameTickState.ThemeWindSpeed = 6.0f;
    env.PrevGameTickState.WindDirection = V(1, 0, 0); env.GameTickState.WindDirection = V(0, 1, 0);
    env.PrevGameTickState.ThemeWindDirection = V(2, 0, 0); env.GameTickState.ThemeWindDirection = V(0, 0, 2);
    env.PrevGameTickState.SunVector = V(3, 0, 0); env.GameTickState.SunVector = V(0, 3, 0);
    env.PrevGameTickState.MoonVector = V(0, 0, 4); env.GameTickState.MoonVector = V(0, 4, 0);
    env.PrevGameTickState.DiffuseVector = V(0, 5, 0); env.GameTickState.DiffuseVector = V(0, 5, 0);

    env.RenderUpdate(0.25f);
    Params& p = env.InterpolatedEnvironmentParams;
    if (g_update_calls != 1) fail = 1;
    if (!near_(p.Time, 12.5f) || !near_(p.WindSpeed, 1.5f) || !near_(p.ThemeWindSpeed, 3.0f)) fail = 2;
    if (!near_(p.WindDirection.X, 0.75f) || !near_(p.WindDirection.Y, 0.25f) || !near_(p.ThemeWindDirection.Z, 0.5f)) fail = 3;
    if (!near_(p.SunVector.X * p.SunVector.X + p.SunVector.Y * p.SunVector.Y + p.SunVector.Z * p.SunVector.Z, 1.0f)) fail = 4;
    if (!near_(p.SunVector.X, 0.948683f) || !near_(p.DiffuseVector.Y, 1.0f)) fail = 5;
    if (p.ChannelCount != 6 || !near_(p.ThemeChannels[3].Scalers[29], 331.5f) || !near_(p.ThemeChannels[5].Colours[18].R, 0.25f)
        || !near_(p.ThemeChannels[2].Colours[7].G, 2.0f) || !near_(p.ThemeChannels[0].Colours[7].B, 7.0f) || p.ThemeChannels[4].TextureSets[2].Textures[0] != 1) fail = 6;
    if (!near_(p.Blur.V[19], 19.25f) || !near_(p.Disp.MonochromeEffectColour.R, 0.25f) || !near_(p.Disp.MonochromeEffectColour.A, 4.0f)) fail = 7;
    if (g_filter_calls != 1 || !near_(g_filter.Alpha, 0.25f) || !near_(g_filter.Saturation, 0.5f) || !near_(g_filter.Brightness, 0.125f)
        || !near_(g_filter.Tint.G, 0.75f) || !near_(g_filter.Tint.A, 0.25f)) fail = 8;

    env.RenderUpdate(1.75f);                                         // clamped to 1: no second Update, the current state wins
    if (g_update_calls != 1 || !near_(p.Time, 20.0f) || !near_(g_last_fraction[0], 1.0f) || !near_(g_last_fraction[2], 1.0f)) fail = 9;
    if (!near_(p.ThemeChannels[1].Scalers[0], 110.0f) || p.ThemeChannels[1].TextureSets[0].Textures[0] != 2) fail = 10;
    env.RenderUpdate(-3.0f);                                         // clamped to 0: the previous state
    if (!near_(p.Time, 10.0f) || !near_(p.WindSpeed, 1.0f) || !near_(g_last_fraction[1], 0.0f) || !near_(p.ThemeChannels[5].Scalers[3], 503.0f)) fail = 11;
    if (!near_(p.MoonVector.Z, 1.0f) || g_filter_calls != 3) fail = 12;

    if (fail) { std::printf("006b42f0_TEST FAIL (%d)\n", fail); return 1; }
    std::printf("006b42f0_TEST PASS\n");
    return 0;
}