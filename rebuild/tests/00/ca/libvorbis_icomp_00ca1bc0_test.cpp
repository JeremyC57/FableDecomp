// Behaviour test for 0x00CA1BC0 icomp (libvorbis floor1.c): qsort comparator over an array of int pointers, ordering by
// the pointed-to value (used by floor1_pack/unpack to sort the post X list).
#include <cstdio>
#include <cstdlib>
extern "C" int icomp(const void* a, const void* b)
{
    return (**(int**)a - **(int**)b);
}
int main()
{
    int vals[6] = { 128, 0, 64, 7, 255, 32 };
    int* ptrs[6];
    int i;
    for (i = 0; i < 6; i++) ptrs[i] = &vals[i];
    std::qsort(ptrs, 6, sizeof(int*), icomp);
    int ok = 1;
    for (i = 1; i < 6; i++) ok &= *ptrs[i - 1] <= *ptrs[i];
    ok &= ptrs[0] == &vals[1] && ptrs[5] == &vals[4];
    ok &= icomp(&ptrs[0], &ptrs[5]) < 0 && icomp(&ptrs[5], &ptrs[0]) > 0 && icomp(&ptrs[2], &ptrs[2]) == 0;
    if (!ok) { std::printf("00ca1bc0_TEST FAIL\n"); return 1; }
    std::printf("00ca1bc0_TEST PASS\n");
    return 0;
}