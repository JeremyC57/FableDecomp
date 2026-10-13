#include <cstdio>

static unsigned long g_fvf = 0;
struct IDev { virtual long __stdcall SetFVF(unsigned long f) { g_fvf = f; return 0; } };
struct RM { IDev* dev; void SetVertexFormat(unsigned long f) { dev->SetFVF(f); } };
int main()
{
    IDev d; RM rm; rm.dev = &d; rm.SetVertexFormat(0x112);
    if (g_fvf != 0x112) { std::printf("00a0aa30_TEST FAIL\n"); return 1; }
    std::printf("00a0aa30_TEST PASS\n");
    return 0;
}