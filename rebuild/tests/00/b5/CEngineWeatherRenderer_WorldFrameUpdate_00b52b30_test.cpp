// Behaviour test for 0x00B52B30: CEngineWeatherRenderer::WorldFrameUpdate strength selection, mist flag, time histories and the clamped wind vector.
#include <cstdio>
#include <cstring>
#include <cmath>
static char g_log[512];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Vec { float X, Y, Z; };
struct Env { float Snow, Rain, Mist; };
struct Globals { Vec WindDirection; float WindSpeed; bool FullFrame; };
static Env g_env;
static Globals g_glob;
struct Weather {
    bool Initialised, ForceWeatherEffects, RainOverriden, SnowOverriden, Mist;
    float OverridenRainStrength, OverridenSnowStrength, CurrentRainStrength, CurrentSnowStrength, MistMaxAlpha;
    float RainSpeedMultiplier, SnowSpeedMultiplier, MistSpeedMultiplier, MistWindSpeedMultiplier, ParticleTimeIncrement;
    float WeatherWindMaxSpeed, WeatherWindMinSpeed, WeatherWindSpeedMultiplier;
    float RainHist[2], SnowHist[2], MistHist[2];
    Vec Wind[2];
    void StartRaining(float) { Log("startRain"); }
    void StartSnowing(float) { Log("startSnow"); }
    void FadeRainToTarget(float) { Log("fadeRain"); }
    void FadeSnowToTarget(float) { Log("fadeSnow"); }
    void Update(bool full)
    {
        if (!Initialised) return;
        if (!ForceWeatherEffects) {
            if (!RainOverriden) {
                float rain = g_env.Rain;
                if (OverridenRainStrength < 0.0f) StartRaining(rain);
                else { FadeRainToTarget(rain); if ((float)std::fabs(CurrentRainStrength - rain) < 0.01f) OverridenRainStrength = -1.0f; }
            } else FadeRainToTarget(OverridenRainStrength);
            if (!SnowOverriden) {
                float snow = g_env.Snow;
                if (OverridenSnowStrength < 0.0f) StartSnowing(snow);
                else { FadeSnowToTarget(snow); if ((float)std::fabs(CurrentSnowStrength - snow) < 0.01f) OverridenSnowStrength = -1.0f; }
            } else FadeSnowToTarget(OverridenSnowStrength);
            float mist = g_env.Mist;
            if (mist <= 0.0f) Mist = false; else { MistMaxAlpha = mist; Mist = true; }
        }
        if (full || g_glob.FullFrame) {
            RainHist[0] = RainHist[1]; SnowHist[0] = SnowHist[1]; MistHist[0] = MistHist[1];
            RainHist[1] += RainSpeedMultiplier * ParticleTimeIncrement;
            Wind[0] = Wind[1];
            SnowHist[1] += SnowSpeedMultiplier * ParticleTimeIncrement;
            MistHist[1] += MistWindSpeedMultiplier * MistSpeedMultiplier * ParticleTimeIncrement;
            float v = g_glob.WindSpeed * WeatherWindSpeedMultiplier;
            v = v < WeatherWindMinSpeed ? WeatherWindMinSpeed : (v > WeatherWindMaxSpeed ? WeatherWindMaxSpeed : v);
            Wind[1].X = g_glob.WindDirection.X * v; Wind[1].Y = g_glob.WindDirection.Y * v; Wind[1].Z = g_glob.WindDirection.Z * v;
        }
    }
};
static void Init(Weather& w)
{
    std::memset(&w, 0, sizeof(w));
    w.Initialised = true; w.OverridenRainStrength = -1.0f; w.OverridenSnowStrength = 0.5f; w.CurrentSnowStrength = 0.305f;
    w.RainSpeedMultiplier = 2.0f; w.SnowSpeedMultiplier = 3.0f; w.MistSpeedMultiplier = 0.5f; w.MistWindSpeedMultiplier = 4.0f; w.ParticleTimeIncrement = 0.25f;
    w.WeatherWindMaxSpeed = 10.0f; w.WeatherWindMinSpeed = 1.0f; w.WeatherWindSpeedMultiplier = 2.0f;
    w.RainHist[1] = 1.0f; w.SnowHist[1] = 2.0f; w.MistHist[1] = 3.0f; w.Wind[1].X = 7.0f;
}
int main()
{
    int bad = 0;
    Weather w;
    g_env.Rain = 0.4f; g_env.Snow = 0.3f; g_env.Mist = 0.6f;
    g_glob.WindDirection.X = 1.0f; g_glob.WindDirection.Y = 0.0f; g_glob.WindDirection.Z = -1.0f; g_glob.WindSpeed = 20.0f; g_glob.FullFrame = false;
    Init(w); g_log[0] = 0; w.Update(true);
    if (std::strcmp(g_log, "startRain;fadeSnow;") != 0 || w.OverridenSnowStrength != -1.0f || !w.Mist || w.MistMaxAlpha != 0.6f) bad |= 1;
    if (w.RainHist[0] != 1.0f || w.RainHist[1] != 1.5f || w.SnowHist[1] != 2.75f || w.MistHist[0] != 3.0f || w.MistHist[1] != 3.5f) bad |= 2;
    if (w.Wind[0].X != 7.0f || w.Wind[1].X != 10.0f || w.Wind[1].Z != -10.0f) bad |= 4;
    Init(w); w.RainOverriden = true; w.SnowOverriden = true; g_env.Mist = 0.0f; g_glob.WindSpeed = 0.1f; g_log[0] = 0; w.Update(false);
    if (std::strcmp(g_log, "fadeRain;fadeSnow;") != 0 || w.Mist || w.RainHist[0] != 0.0f) bad |= 8;
    g_glob.FullFrame = true; w.ForceWeatherEffects = true; g_log[0] = 0; w.Update(false);
    if (g_log[0] != 0 || w.Wind[1].X != 1.0f || w.Wind[1].Z != -1.0f) bad |= 16;
    w.Initialised = false; w.RainHist[1] = 9.0f; w.Update(true);
    if (w.RainHist[0] == 9.0f) bad |= 32;
    if (bad) { std::printf("00b52b30_TEST FAIL %d\n", bad); return 1; }
    std::printf("00b52b30_TEST PASS\n");
    return 0;
}