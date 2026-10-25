// Behaviour test for 0x00C95560 libvorbis sort32a (copy of the logic, driven through qsort).
#include <cstdio>
#include <cstdlib>
typedef unsigned int u32;
extern "C" int sort32a_t(const void* a, const void* b)
{
    return (**(u32**)a > **(u32**)b) - (**(u32**)a < **(u32**)b);
}
int main()
{
    u32 v[5] = { 7, 0xFFFFFFFFu, 0, 0x80000000u, 7 };
    u32* w[5] = { v, v + 1, v + 2, v + 3, v + 4 };
    std::qsort(w, 5, sizeof(u32*), sort32a_t);
    int ok = *w[0] == 0 && *w[1] == 7 && *w[2] == 7 && *w[3] == 0x80000000u && *w[4] == 0xFFFFFFFFu;
    if (sort32a_t(&w[1], &w[2]) != 0 || sort32a_t(&w[4], &w[0]) != 1 || sort32a_t(&w[0], &w[3]) != -1) ok = 0;
    if (!ok) { std::printf("00c95560_TEST FAIL\n"); return 1; }
    std::printf("00c95560_TEST PASS\n");
    return 0;
}