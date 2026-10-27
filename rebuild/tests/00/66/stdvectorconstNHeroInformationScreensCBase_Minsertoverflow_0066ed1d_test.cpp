// behaviour test for 0x0066ED1D vector<const NHeroInformationScreens::CBase*>::_M_insert_overflow (model: head, n copies of x, tail into a grown block)
#include <cstdio>
#include <cstdlib>
#include <cstring>
int main()
{
    int a = 1, b = 2, c = 3, x = 9;
    int* old[3] = { &a, &b, &c };
    unsigned int oldSize = 3, n = 2, len = oldSize + (oldSize > n ? oldSize : n);
    int** nb = (int**)std::malloc(len * sizeof(int*)); int** f = nb;
    std::memmove(f, old, 1 * sizeof(int*)); f += 1;
    for (unsigned int i = 0; i < n; ++i) *f++ = &x;
    std::memmove(f, old + 1, 2 * sizeof(int*)); f += 2;
    int ok = len == 6 && f - nb == 5 && nb[0] == &a && nb[1] == &x && nb[2] == &x && nb[3] == &b && nb[4] == &c;
    std::free(nb);
    if (!ok) { std::printf("0066ed1d_TEST FAIL\n"); return 1; }
    std::printf("0066ed1d_TEST PASS\n");
    return 0;
}