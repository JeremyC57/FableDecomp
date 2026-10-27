#include <cstdio>
#include <cstdlib>
#include <cstring>
// floor1_free_look: wipe the 328-int look, then free it; a null look is ignored.
static int g_frees = 0;
static void* g_last = 0;
static int g_wiped = 0;
static void counting_free(void* p) { int* q = (int*)p; g_wiped = 1; for (int i = 0; i < 328; ++i) if (q[i]) g_wiped = 0; ++g_frees; g_last = p; std::free(p); }
static void floor1_free_look(void* i)
{
    int* look = (int*)i;
    if (look) {
        std::memset(look, 0, 328 * sizeof(int));
        counting_free(look);
    }
}
int main()
{
    floor1_free_look(0);
    if (g_frees != 0) { std::printf("00ca1770_TEST FAIL\n"); return 1; }
    int* look = (int*)std::malloc(328 * sizeof(int));
    for (int i = 0; i < 328; ++i) look[i] = i + 1;
    floor1_free_look(look);
    if (g_frees != 1 || g_last != look || !g_wiped) { std::printf("00ca1770_TEST FAIL\n"); return 1; }
    std::printf("00ca1770_TEST PASS\n");
    return 0;
}