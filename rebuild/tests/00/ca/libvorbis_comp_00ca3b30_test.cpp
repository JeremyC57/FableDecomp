// Behaviour test for 0x00CA3B30 comp (libvorbis lsp.c): qsort comparator that orders floats from largest to smallest
// (used to sort the LSP roots).
#include <cstdio>
#include <cstdlib>
extern "C" int comp(const void* a, const void* b)
{
    return (*(float*)a < *(float*)b) - (*(float*)a > *(float*)b);
}
int main()
{
    float v[7] = { 0.5f, -1.25f, 3.0f, 0.0f, 2.75f, -0.5f, 3.0f };
    std::qsort(v, 7, sizeof(float), comp);
    int ok = 1;
    int i;
    for (i = 1; i < 7; i++) ok &= v[i - 1] >= v[i];
    ok &= v[0] == 3.0f && v[6] == -1.25f;
    float a = 1.0f, b = 2.0f;
    ok &= comp(&a, &b) == 1 && comp(&b, &a) == -1 && comp(&a, &a) == 0;
    if (!ok) { std::printf("00ca3b30_TEST FAIL\n"); return 1; }
    std::printf("00ca3b30_TEST PASS\n");
    return 0;
}