// behaviour test for 00c16720 (standalone copy of the model)
#include <cstdio>
#include <cstring>

#include <cstdlib>
static void* g_freed = 0;
struct A { virtual ~A() {} virtual void Free(void* p) { g_freed = p; std::free(p); } };

int main()
{
    A al; void* p = std::malloc(8); al.Free(p);
    if (!(g_freed == p)) { std::printf("00c16720_TEST FAIL\n"); return 1; }
    al.Free(0);
    if (!(g_freed == 0)) { std::printf("00c16720_TEST FAIL\n"); return 1; }
    std::printf("00c16720_TEST PASS\n");
    return 0;
}