// Behaviour test for 00c8d450: ogg_sync_destroy -- clears then frees a non-null sync state, returns 0
#include <cstdio>
#include <cstdlib>
static int g_cleared = 0, g_freed = 0;
struct ogg_sync_state { unsigned char body[0x1c]; };
static int ogg_sync_clear(ogg_sync_state*) { ++g_cleared; return 0; }
static void my_free(void* p) { ++g_freed; std::free(p); }
static int ogg_sync_destroy(ogg_sync_state* os) { if (os) { ogg_sync_clear(os); my_free(os); } return (0); }
int main()
{
    int ok = ogg_sync_destroy((ogg_sync_state*)std::malloc(sizeof(ogg_sync_state))) == 0 && g_cleared == 1 && g_freed == 1;
    ok = ok && ogg_sync_destroy(0) == 0 && g_cleared == 1 && g_freed == 1;
    std::printf(ok ? "00c8d450_TEST PASS\n" : "00c8d450_TEST FAIL\n");
    return ok ? 0 : 1;
}
