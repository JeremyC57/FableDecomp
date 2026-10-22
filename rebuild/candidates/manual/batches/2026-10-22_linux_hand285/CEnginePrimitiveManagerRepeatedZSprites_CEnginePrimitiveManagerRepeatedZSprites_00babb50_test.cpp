#include <cstdio>
#include <cstring>
// Model of the repeated z-sprite manager constructor: shader lookups by name, the enable switch, buffer build,
// texture page + 256x256 billboard texture (8888 format) + depth surface (format 0x4b), then the defaults.
struct Size { long X, Y; Size(long x, long y) : X(x), Y(y) { } };
static int g_calls[8];
static long g_lastW = 0, g_lastH = 0, g_lastFormat = 0, g_lastPage = -2;
static const char* g_names[4];
static int g_nameCount = 0;
static long Lookup(const char* name) { g_names[g_nameCount++] = name; return g_nameCount; }
static long AllocPage(long a, long b) { ++g_calls[0]; return (a == 0 && b == 1) ? 7 : -1; }
static long AllocTexture(long page, const Size& size, long mips, long format, long count, long flags)
{
    ++g_calls[1]; g_lastPage = page; g_lastW = size.X; g_lastH = size.Y; g_lastFormat = format;
    return (mips == 1 && count == 1 && flags == 0) ? 11 : -1;
}
static long AllocDepth(long page, const Size& size, long format, long flags)
{
    ++g_calls[2]; g_lastPage = page; g_lastW = size.X; g_lastH = size.Y; g_lastFormat = format;
    return flags == 0 ? 12 : -1;
}
struct CMgr {
    long VS, PS, PSNoStipple;
    void* SortStart; void* SortFinish; void* SortEnd;
    void* BillboardTexture; void* BillboardSurface;
    long BillboardHandle, ZBufferHandle, PageHandle;
    float NonStippleAlpha; bool EnableAlpha; float Vertical; float Power; void* Mesh;
    bool Built;
    CMgr() : SortStart(0), SortFinish(0), SortEnd(0), Built(false)
    {
        VS = Lookup("VSHADER_ZSPRITE");
        PS = Lookup("PSHADER_ZSPRITE");
        PSNoStipple = Lookup("PSHADER_ZSPRITE_NO_STIPPLE");
        Lookup("EnableZSprites");
        Built = true;
        PageHandle = AllocPage(0, 1);
        BillboardHandle = AllocTexture(PageHandle, Size(256, 256), 1, 8888, 1, 0);
        BillboardTexture = 0; BillboardSurface = 0;
        ZBufferHandle = AllocDepth(PageHandle, Size(256, 256), 0x4b, 0);
        Mesh = 0;
        NonStippleAlpha = 1.2f; EnableAlpha = true; Vertical = 5.0f; Power = 1.0f;
    }
};
int main()
{
    int bad = 0;
    CMgr m;
    if (g_nameCount != 4 || std::strcmp(g_names[0], "VSHADER_ZSPRITE") != 0 || std::strcmp(g_names[2], "PSHADER_ZSPRITE_NO_STIPPLE") != 0
        || std::strcmp(g_names[3], "EnableZSprites") != 0) bad = 1;
    if (m.VS != 1 || m.PS != 2 || m.PSNoStipple != 3 || !m.Built) bad = 1;
    if (m.PageHandle != 7 || m.BillboardHandle != 11 || m.ZBufferHandle != 12) bad = 1;
    if (g_lastPage != 7 || g_lastW != 256 || g_lastH != 256 || g_lastFormat != 0x4b) bad = 1;
    if (g_calls[0] != 1 || g_calls[1] != 1 || g_calls[2] != 1) bad = 1;
    if (m.SortStart || m.SortFinish || m.SortEnd || m.Mesh || m.BillboardTexture || m.BillboardSurface) bad = 1;
    if (m.NonStippleAlpha != 1.2f || !m.EnableAlpha || m.Vertical != 5.0f || m.Power != 1.0f) bad = 1;
    if (bad) { std::printf("00babb50_TEST FAIL\n"); return 1; }
    std::printf("00babb50_TEST PASS\n");
    return 0;
}
