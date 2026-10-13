#include <cstdio>
static void* g_seen = 0; extern "C" void __cdecl cb(void* p) { g_seen = p; }
static void __stdcall Fwd(void* p) { cb(p); }
int main()
{
    int x; Fwd(&x); if (g_seen != &x) { std::printf("00c33470_TEST FAIL\n"); return 1; }

    std::printf("00c33470_TEST PASS\n");
    return 0;
}