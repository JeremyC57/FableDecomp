#include <cstdio>
struct Res {};
static void* g_blocks[8]; static Res* g_h[8]; static int g_n = 0;
struct Mgr { void SetMovableResourceHandler(void* p, Res* h) { g_blocks[g_n] = p; g_h[g_n] = h; ++g_n; } };
static Mgr g_mgr;
template <class T> struct Arr {
    T* Data; long Count;
    void SetMovableResourceHandler(Res* h) { if (Data) g_mgr.SetMovableResourceHandler(Data, h); }
};
struct Strip {
    unsigned short StartPos, Size;
    Arr<int> Verts; Arr<int> Fan; Arr<int> Water; Arr<int> WFan;
    void SetMovableResourceHandler(Res* h)
    {
        Verts.SetMovableResourceHandler(h); Fan.SetMovableResourceHandler(h);
        Water.SetMovableResourceHandler(h); WFan.SetMovableResourceHandler(h);
    }
};
int main()
{
    static int a, c, d; Res r;
    Strip s; s.Verts.Data = &a; s.Fan.Data = 0; s.Water.Data = &c; s.WFan.Data = &d;
    s.SetMovableResourceHandler(&r);
    if (g_n != 3 || g_blocks[0] != &a || g_blocks[1] != &c || g_blocks[2] != &d || g_h[2] != &r) { std::printf("00bfc510_TEST FAIL\n"); return 1; }
    std::printf("00bfc510_TEST PASS\n");
    return 0;
}