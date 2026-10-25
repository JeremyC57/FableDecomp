// Behaviour test for 00c8ced0: ogg_stream_destroy -- clears then frees a non-null stream state, returns 0
#include <cstdio>
#include <cstdlib>
static int g_cleared = 0, g_freed = 0;
struct ogg_stream_state { unsigned char body[0x168]; };
static int ogg_stream_clear(ogg_stream_state*) { ++g_cleared; return 0; }
static void my_free(void* p) { ++g_freed; std::free(p); }
static int ogg_stream_destroy(ogg_stream_state* os) { if (os) { ogg_stream_clear(os); my_free(os); } return (0); }
int main()
{
    int ok = ogg_stream_destroy((ogg_stream_state*)std::malloc(sizeof(ogg_stream_state))) == 0 && g_cleared == 1 && g_freed == 1;
    ok = ok && ogg_stream_destroy(0) == 0 && g_cleared == 1 && g_freed == 1;
    std::printf(ok ? "00c8ced0_TEST PASS\n" : "00c8ced0_TEST FAIL\n");
    return ok ? 0 : 1;
}
