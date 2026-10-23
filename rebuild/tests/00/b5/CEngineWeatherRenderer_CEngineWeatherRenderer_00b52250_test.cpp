#include <cstdio>
#include <cstring>
// Model of the weather renderer constructor: mist, snow and rain shaders looked up by name, the particle buffers
// built, then no overrides (-1 strengths), mask 0 and a full particle percentage.
struct Shader { const char* name; char kind; };
static const char* g_names[32]; static char g_kinds[32]; static int g_n = 0;
static int g_built = 0; static const char* g_enable = 0;
static Shader Lookup(char kind, const char* name)
{
    Shader s; s.name = name; s.kind = kind;
    g_names[g_n] = name; g_kinds[g_n] = kind; ++g_n;
    return s;
}
static Shader GetVertexShader(const char* name) { return Lookup('V', name); }
static Shader GetPixelShader(const char* name) { return Lookup('P', name); }
static void RegisterEnable(const char* name) { g_enable = name; }
static bool Is(const Shader& s, char kind, const char* name) { return s.kind == kind && s.name && std::strcmp(s.name, name) == 0; }
struct Manager {
    Shader MistVertexShader;
    Shader MistPixelShader;
    Shader SnowParticleVertexShader;
    Shader SnowParticleClippingVertexShader;
    Shader SnowParticlePixelShader;
    Shader RainParticleVertexShader;
    Shader RainParticleClippingVertexShader;
    Shader RainParticlePixelShader;
    float OverridenRainStrength;
    float OverridenSnowStrength;
    long CurrentMask;
    bool RainOverriden;
    bool SnowOverriden;
    float MaximumWeatherParticlePercentage;
    Manager()
    {
        CurrentMask = 7; RainOverriden = true; SnowOverriden = true;
        MistVertexShader = GetVertexShader("VSHADER_MIST");
        MistPixelShader = GetPixelShader("PSHADER_MIST");
        SnowParticleVertexShader = GetVertexShader("VSHADER_WEATHER_SNOW");
        SnowParticleClippingVertexShader = GetVertexShader("VSHADER_WEATHER_SNOW_CLIPPING");
        SnowParticlePixelShader = GetPixelShader("PSHADER_WEATHER_SNOW");
        RainParticleVertexShader = GetVertexShader("VSHADER_WEATHER_RAIN");
        RainParticleClippingVertexShader = GetVertexShader("VSHADER_WEATHER_RAIN_CLIPPING");
        RainParticlePixelShader = GetPixelShader("PSHADER_WEATHER_RAIN");
        ++g_built;
        OverridenRainStrength = -1.0f; OverridenSnowStrength = -1.0f; CurrentMask = 0;
        RainOverriden = false; SnowOverriden = false; MaximumWeatherParticlePercentage = 1.0f;
    }
};
int main()
{
    int bad = 0;
    Manager m;
    static const char* expected[8] = { "VSHADER_MIST", "PSHADER_MIST", "VSHADER_WEATHER_SNOW", "VSHADER_WEATHER_SNOW_CLIPPING", "PSHADER_WEATHER_SNOW", "VSHADER_WEATHER_RAIN", "VSHADER_WEATHER_RAIN_CLIPPING", "PSHADER_WEATHER_RAIN" };
    if (g_n != 8) bad = 1;
    for (int i = 0; i < g_n && i < 8; ++i) if (std::strcmp(g_names[i], expected[i]) != 0) bad = 1;
    if (!Is(m.MistVertexShader, 'V', "VSHADER_MIST")) bad = 1;
    if (!Is(m.MistPixelShader, 'P', "PSHADER_MIST")) bad = 1;
    if (!Is(m.SnowParticleVertexShader, 'V', "VSHADER_WEATHER_SNOW")) bad = 1;
    if (!Is(m.SnowParticleClippingVertexShader, 'V', "VSHADER_WEATHER_SNOW_CLIPPING")) bad = 1;
    if (!Is(m.SnowParticlePixelShader, 'P', "PSHADER_WEATHER_SNOW")) bad = 1;
    if (!Is(m.RainParticleVertexShader, 'V', "VSHADER_WEATHER_RAIN")) bad = 1;
    if (!Is(m.RainParticleClippingVertexShader, 'V', "VSHADER_WEATHER_RAIN_CLIPPING")) bad = 1;
    if (!Is(m.RainParticlePixelShader, 'P', "PSHADER_WEATHER_RAIN")) bad = 1;
    if (g_built != 1) bad = 1;
    if (m.OverridenRainStrength != -1.0f || m.OverridenSnowStrength != -1.0f || m.CurrentMask != 0 || m.RainOverriden || m.SnowOverriden || m.MaximumWeatherParticlePercentage != 1.0f) bad = 1;
    if (bad) { std::printf("00b52250_TEST FAIL\n"); return 1; }
    std::printf("00b52250_TEST PASS\n");
    return 0;
}