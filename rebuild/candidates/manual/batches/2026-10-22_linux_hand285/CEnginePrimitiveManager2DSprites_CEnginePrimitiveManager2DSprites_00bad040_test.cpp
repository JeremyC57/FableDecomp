#include <cstdio>
#include <cstring>
// Model: ref-counted shader handles looked up by name, assigned into the manager; index list 0,1,2,1,3,2.
struct SData { long refs; const char* name; };
static SData g_pool[4];
static int g_released = 0;
struct CShader {
    SData* Data;
    CShader() : Data(0) { }
    CShader(const CShader& o) : Data(o.Data) { if (Data) ++Data->refs; }
    CShader& operator=(const CShader& o)
    {
        if (Data) { if (--Data->refs == 0) ++g_released; Data = 0; }
        Data = o.Data;
        ++Data->refs;
        return *this;
    }
    ~CShader() { if (Data) { if (--Data->refs == 0) ++g_released; } }
};
static CShader Lookup(const char* name)
{
    static const char* names[4] = { "VSHADER_2D_SPRITE", "VSHADER_2D_CLOCK_SPRITE", "PSHADER_2D_CLOCK_SPRITE", "PSHADER_2D_CLOCK_SPRITE_ADDITIVE" };
    CShader s;
    for (int i = 0; i < 4; ++i)
        if (std::strcmp(names[i], name) == 0) { s.Data = &g_pool[i]; ++g_pool[i].refs; g_pool[i].name = names[i]; }
    return s;
}
static const char* g_enabled = 0;
struct CMgr {
    CShader VS, VSClock, PSClock, PSAdd;
    unsigned short Indices[6];
    CMgr()
    {
        VS = Lookup("VSHADER_2D_SPRITE");
        VSClock = Lookup("VSHADER_2D_CLOCK_SPRITE");
        PSClock = Lookup("PSHADER_2D_CLOCK_SPRITE");
        PSAdd = Lookup("PSHADER_2D_CLOCK_SPRITE_ADDITIVE");
        Indices[0] = 0; Indices[1] = 1; Indices[2] = 2; Indices[3] = 1; Indices[4] = 3; Indices[5] = 2;
        g_enabled = "Enable2DSprites";
    }
};
int main()
{
    int bad = 0;
    {
        CMgr m;
        static const unsigned short want[6] = { 0, 1, 2, 1, 3, 2 };
        for (int i = 0; i < 6; ++i) if (m.Indices[i] != want[i]) bad = 1;
        for (int i = 0; i < 4; ++i) if (g_pool[i].refs != 1) bad = 1;
        if (m.PSAdd.Data != &g_pool[3] || m.VS.Data != &g_pool[0]) bad = 1;
        if (!g_enabled || std::strcmp(g_enabled, "Enable2DSprites") != 0) bad = 1;
        if (g_released != 0) bad = 1;
    }
    if (g_released != 4) bad = 1;
    if (bad) { std::printf("00bad040_TEST FAIL\n"); return 1; }
    std::printf("00bad040_TEST PASS\n");
    return 0;
}
