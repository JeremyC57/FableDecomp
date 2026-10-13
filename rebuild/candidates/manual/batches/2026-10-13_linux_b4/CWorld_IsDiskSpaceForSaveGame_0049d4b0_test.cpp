#include <cstdio>

static int g_dtor = 0; static long g_size = 0;
struct WS { void* p; ~WS() { ++g_dtor; } };
struct Mgr { WS GetDirectoryNameFromProfileName() { WS w; w.p = (void*)5; return w; } };
static Mgr g_mgr;
Mgr* __fastcall Mgr_Get() { return &g_mgr; }
bool __fastcall IsRoomForFile(long n, WS* d) { g_size = n; return d->p == (void*)5; }
struct W { bool Is() { WS d = Mgr_Get()->GetDirectoryNameFromProfileName(); return IsRoomForFile(0x96000, &d); } };
int main()
{
    W w; bool r = w.Is();
    if (!r || g_size != 0x96000 || g_dtor < 1) { std::printf("0049d4b0_TEST FAIL\n"); return 1; }
    std::printf("0049d4b0_TEST PASS\n");
    return 0;
}
