// behaviour test for 0x0056FF81: the accessor returns the interface's component when the presence bit is set
#include <cstdio>
struct E { long type; void* comp; };
static void* Lookup(E* f, E* l, long t) { E* lo = f; int len = (int)(l - f); while (len > 0) { int h = len >> 1; E* m = lo + h; if (m->type < t) { lo = m + 1; len = len - h - 1; } else len = h; } E* it = (lo != l && !(t < lo->type)) ? lo : l; return it->comp; }
int main()
{
    int a, b, c; E map[4] = { { 4, &a }, { 0x68, &b }, { 0x6f, &c }, { 0, 0 } };
    if (Lookup(map, map + 3, 0x68) != &b || Lookup(map, map + 3, 4) != &a || Lookup(map, map + 3, 0x6f) != &c) { std::printf("0056ff81_TEST FAIL\n"); return 1; }
    std::printf("0056ff81_TEST PASS\n");
    return 0;
}