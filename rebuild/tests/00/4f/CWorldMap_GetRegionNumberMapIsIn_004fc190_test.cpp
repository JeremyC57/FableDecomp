#include <cstdio>
#include <cstring>
struct Region { long maps[2]; bool Contains(long m) const { return maps[0] == m || maps[1] == m; } };
static long Find(const Region* r, unsigned long n, long map) { for (unsigned long i = 1; i < n; ++i) if (r[i].Contains(map)) return (long)i; return 0; }
static bool Run() { Region r[3] = { { { 1, 2 } }, { { 3, 4 } }, { { 5, 1 } } }; return Find(r, 3, 1) == 2 && Find(r, 3, 4) == 1 && Find(r, 3, 2) == 0; }
int main()
{
    if (!Run()) { std::printf("004fc190_TEST FAIL\n"); return 1; }
    std::printf("004fc190_TEST PASS\n");
    return 0;
}