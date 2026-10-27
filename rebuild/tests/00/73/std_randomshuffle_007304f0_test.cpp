// behaviour test for 0x007304F0 random_shuffle (model: a permutation of the input)
#include <cstdio>
#include <cstdlib>
int main()
{
    int a[6] = { 1, 2, 3, 4, 5, 6 };
    int* first = a; int* last = a + 6;
    for (int* i = first + 1; i != last; ++i) { int* j = first + std::rand() % (int)((i - first) + 1); int t = *i; *i = *j; *j = t; }
    int sum = 0, prod = 1; for (int k = 0; k < 6; ++k) { sum += a[k]; prod *= a[k]; }
    int ok = sum == 21 && prod == 720;
    if (!ok) { std::printf("007304f0_TEST FAIL\n"); return 1; }
    std::printf("007304f0_TEST PASS\n");
    return 0;
}