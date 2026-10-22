// Behaviour test for CEngineScreenEffectColourFilter::CEngineScreenEffectColourFilter (0x00B5A1D0): support follows the caps
// bit (clearing Enabled when unsupported), grey weights / level, the full-screen quad, and the shader assignments that
// release the old shader data and add a reference to the new.
#include <cstdio>

struct ShaderData { long refs; bool deleted; };
struct Shader {
    ShaderData* Data;
    Shader() : Data(0) { }
    explicit Shader(ShaderData* d) : Data(d) { }
    Shader& operator=(const Shader& other)
    {
        ShaderData* data = Data;
        if (data) { if (--data->refs == 0) data->deleted = true; }
        Data = other.Data;
        ++Data->refs;
        return *this;
    }
    ~Shader() { if (Data) { --Data->refs; if (Data->refs == 0) Data->deleted = true; } }
};
static ShaderData g_vs = { 1, false }, g_ps = { 1, false };
static Shader GetShader(ShaderData* d) { ++d->refs; return Shader(d); }
struct Vert { float X, Y, Z; };
struct Filter {
    bool Enabled, ForceColourFilter, EffectSupported;
    float Weights[4]; float GrayLevel; Vert Verts[4]; Shader VS, PS;
    Filter(bool supported) : Enabled(true), ForceColourFilter(true)
    {
        if (supported) EffectSupported = true; else { EffectSupported = false; Enabled = false; }
        Weights[0] = 0.212671f; Weights[1] = 0.715160f; Weights[2] = 0.072169f; Weights[3] = 1.0f;
        ForceColourFilter = false;
        GrayLevel = 0.333f;
        const float xs[4] = { -1.0f, 1.0f, 1.0f, -1.0f }, ys[4] = { -1.0f, -1.0f, 1.0f, 1.0f };
        for (int i = 0; i < 4; ++i) { Verts[i].X = xs[i]; Verts[i].Y = ys[i]; Verts[i].Z = 0.0f; }
        VS = GetShader(&g_vs);
        PS = GetShader(&g_ps);
    }
};
int main()
{
    bool ok = true;
    {
        Filter f(true);
        ok = ok && f.EffectSupported && f.Enabled && !f.ForceColourFilter && f.GrayLevel == 0.333f;
        ok = ok && f.Verts[2].X == 1.0f && f.Verts[3].Y == 1.0f && f.Verts[0].Z == 0.0f;
        ok = ok && f.VS.Data == &g_vs && g_vs.refs == 2 && g_ps.refs == 2 && !g_vs.deleted;
    }
    ok = ok && g_vs.refs == 1 && g_ps.refs == 1;
    {
        Filter f(false);
        ok = ok && !f.EffectSupported && !f.Enabled;
    }
    if (!ok) { std::printf("00b5a1d0_TEST FAIL\n"); return 1; }
    std::printf("00b5a1d0_TEST PASS\n");
    return 0;
}