#include <cstdio>

struct Init { unsigned char Raw[8]; Init() { for (int i = 0; i < 8; ++i) Raw[i] = (unsigned char)(i + 1); } };
struct Fmt { long idx; Init GetInit() { return Init(); } };
static long g_n = 0; static unsigned char g_first = 0;
struct Out { virtual void v00(){} virtual void v04(){} virtual void v08(){} virtual void v0c(){}
    virtual void WriteBytes(const void* d, long n) { g_n = n; g_first = *(const unsigned char*)d; }
    void WritePixelFormat(Fmt* f) { Init i = f->GetInit(); WriteBytes(&i, 6); } };
int main()
{
    Fmt f; Out o; o.WritePixelFormat(&f);
    if (g_n != 6 || g_first != 1) { std::printf("00993f60_TEST FAIL\n"); return 1; }
    std::printf("00993f60_TEST PASS\n");
    return 0;
}
