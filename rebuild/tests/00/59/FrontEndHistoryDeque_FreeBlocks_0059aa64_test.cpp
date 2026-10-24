// Behaviour test for 0x0059AA64 _Deque_base::_M_destroy_nodes: frees every non-null block in [start, finish).
#include <cstdio>
#include <cstdlib>
static int g_freed = 0;
static void TrackedFree(void* p) { ++g_freed; std::free(p); }
struct Alloc { void deallocate(int* p, size_t) { if (p) TrackedFree(p); } };
struct DequeBase {
    Alloc a;
    void DestroyNodes(int** start, int** finish) { for (int** n = start; n < finish; ++n) a.deallocate(*n, 32); }
};
int main()
{
    int* map[4];
    map[0] = (int*)std::malloc(4); map[1] = 0; map[2] = (int*)std::malloc(4); map[3] = (int*)std::malloc(4);
    DequeBase d;
    d.DestroyNodes(map, map + 3);
    if (g_freed != 2) { std::printf("0059aa64_TEST FAIL\n"); return 1; }
    d.DestroyNodes(map + 3, map + 3);
    if (g_freed != 2) { std::printf("0059aa64_TEST FAIL\n"); return 1; }
    d.DestroyNodes(map + 3, map + 4);
    if (g_freed != 3) { std::printf("0059aa64_TEST FAIL\n"); return 1; }
    std::printf("0059aa64_TEST PASS\n");
    return 0;
}
