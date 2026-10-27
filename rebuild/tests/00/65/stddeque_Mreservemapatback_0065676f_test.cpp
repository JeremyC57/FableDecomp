// behaviour test for 0x0065676F: the map is reallocated only when fewer than n + 1 slots remain behind finish
#include <cstdio>
static int g_realloc = 0;
static void Reserve(unsigned int n, unsigned int mapSize, unsigned int finishIndex) { if (n + 1 > mapSize - finishIndex) ++g_realloc; }
int main()
{
    Reserve(1, 8, 3); int ok = g_realloc == 0;
    Reserve(1, 8, 7); ok = ok && g_realloc == 1;
    Reserve(4, 8, 4); ok = ok && g_realloc == 2;
    if (!ok) { std::printf("0065676f_TEST FAIL\n"); return 1; }
    std::printf("0065676f_TEST PASS\n");
    return 0;
}