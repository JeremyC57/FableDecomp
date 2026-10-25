// Behaviour test for 00ca1750: libvorbis floor1_free_info -- a null setup is ignored; a setup is wiped (1120 bytes) and freed
#include <cstdio>
#include <cstring>
#include <cstdlib>
static int g_frees = 0; static unsigned char g_seen[1120]; static void* g_last = 0;
static void ogg_free_spy(void* p) { ++g_frees; g_last = p; std::memcpy(g_seen, p, 1120); std::free(p); }
static void __fastcall floor1_free_info(void* i) { if (i) { std::memset(i, 0, 1120); ogg_free_spy(i); } }
int main()
{
    floor1_free_info(0);
    void* p = std::malloc(1120); std::memset(p, 0x5a, 1120);
    floor1_free_info(p);
    int zero = 1; for (int k = 0; k < 1120; ++k) if (g_seen[k]) zero = 0;
    int ok = g_frees == 1 && g_last == p && zero;
    std::printf(ok ? "00ca1750_TEST PASS\n" : "00ca1750_TEST FAIL\n");
    return ok ? 0 : 1;
}
