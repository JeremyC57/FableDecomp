#include <cstdio>
struct Res {};
static unsigned int g_log[8]; static int g_n = 0;
template <class T> struct Arr {
    T* Data; long Count;
    Arr() : Data(0), Count(0) {}
    bool Resize(long n, Res*) { Count = n; g_log[g_n++] = (unsigned int)sizeof(T); return true; }
};
struct V1 { char a[1]; }; struct V2 { char a[2]; }; struct V3 { char a[3]; }; struct V4 { char a[4]; };
struct Strip {
    unsigned short StartPos, Size;
    Arr<V1> Verts; Arr<V2> Fan; Arr<V3> Water; Arr<V4> WFan;
    bool Tesselated, Vertical;
    Strip(unsigned short startPos, long size, bool vertical, Res* r)
    {
        Size = (unsigned short)size; StartPos = startPos;
        Verts.Resize(size + 1, r); Water.Resize(size + 1, r); Fan.Resize(size + 1, r); WFan.Resize(size + 1, r);
        Vertical = vertical; Tesselated = false;
    }
};
int main()
{
    Res r; Strip s(3, 5, true, &r);
    int ok = s.StartPos == 3 && s.Size == 5 && s.Vertical && !s.Tesselated && s.Verts.Count == 6 && s.WFan.Count == 6
          && g_n == 4 && g_log[0] == 1 && g_log[1] == 3 && g_log[2] == 2 && g_log[3] == 4;
    if (!ok) { std::printf("00bfc2e0_TEST FAIL\n"); return 1; }
    std::printf("00bfc2e0_TEST PASS\n");
    return 0;
}