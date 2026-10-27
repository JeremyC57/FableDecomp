// behaviour test for 0x00913A80: the map growth/centring arithmetic of _M_reallocate_map
#include <cstdio>
#include <cstdlib>
#include <cstring>
static unsigned int NewMapSize(unsigned int size, unsigned int add) { return size + (size > add ? size : add) + 2; }
static unsigned int NewStart(unsigned int mapSize, unsigned int nodes, unsigned int add, bool front) { return (mapSize - nodes) / 2 + (front ? add : 0); }
int main()
{
    int ok = NewMapSize(8, 1) == 18 && NewMapSize(8, 20) == 30 && NewStart(18, 5, 1, true) == 7 && NewStart(18, 5, 1, false) == 6;
    if (!ok) { std::printf("00913a80_TEST FAIL\n"); return 1; }
    std::printf("00913a80_TEST PASS\n");
    return 0;
}