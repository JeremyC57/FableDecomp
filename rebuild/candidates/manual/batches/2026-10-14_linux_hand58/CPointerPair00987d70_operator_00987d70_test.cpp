#include <cstdio>
#include <cstring>
#include <cstdlib>

struct CPair { void* vt; void* item; long tag; long operator!=(const CPair& o) const { if (item == o.item && tag == o.tag) return 0; return 1; } };
int main()
{
    int x, y; CPair a, b; a.item = &x; a.tag = 1; b.item = &x; b.tag = 1;
    if ((a != b) != 0) { std::printf("00987d70_TEST FAIL\n"); return 1; }
    b.tag = 2; if ((a != b) != 1) { std::printf("00987d70_TEST FAIL\n"); return 1; }
    b.tag = 1; b.item = &y; if ((a != b) != 1) { std::printf("00987d70_TEST FAIL\n"); return 1; }
    std::printf("00987d70_TEST PASS\n");
    return 0;
}
