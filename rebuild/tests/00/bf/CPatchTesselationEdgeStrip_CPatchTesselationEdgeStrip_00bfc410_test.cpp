#include <cstdio>
static int g_freed[8]; static int g_nfreed = 0;
struct Mgr { void FreeVirtual(void* p) { g_freed[g_nfreed++] = *(int*)p; } };
static Mgr g_mgr;
template <class T> struct Arr {
    T* Data; long Count;
    Arr() : Data(0), Count(0) {}
    ~Arr() { if (Data) g_mgr.FreeVirtual(Data); }
};
struct Strip {
    unsigned short StartPos, Size;
    Arr<int> Verts; Arr<int> Fan; Arr<int> Water; Arr<int> WFan;
    bool Tesselated, Vertical;
    ~Strip() {}
};
int main()
{
    static int a = 1, b = 2, d = 4;
    {
        Strip s; s.Verts.Data = &a; s.Fan.Data = &b; s.WFan.Data = &d;
    }
    if (g_nfreed != 3 || g_freed[0] != 4 || g_freed[1] != 2 || g_freed[2] != 1) { std::printf("00bfc410_TEST FAIL\n"); return 1; }
    std::printf("00bfc410_TEST PASS\n");
    return 0;
}