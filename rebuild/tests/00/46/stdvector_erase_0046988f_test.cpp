// behaviour test for 0x0046988F vector<ECreatureGeneratorGenerateType >::erase(first, last): the tail is copied down onto first element by element (front to back, so overlapping ranges are safe), finish moves back by last - first, first is returned; an empty range leaves the contents and erase(begin, end) empties the vector
#include <cstdio>
struct Vec { unsigned int* s; unsigned int* f; unsigned int* e; };
static unsigned int* Erase(Vec& v, unsigned int* first, unsigned int* last)
{
    unsigned int* r = first;
    for (int n = (int)(v.f - last); n > 0; --n) { *r = *last; ++last; ++r; }
    v.f = r;
    return first;
}
int main()
{
    int fails = 0;
    unsigned int a[8] = { 10u, 11u, 12u, 13u, 14u, 15u, 16u, 0xDEADu };
    Vec v = { a, a + 7, a + 8 };
    unsigned int* r = Erase(v, a + 1, a + 4);          // drop 11, 12, 13
    if (r != a + 1 || v.f != a + 4 || v.e != a + 8) fails |= 1;
    if (a[0] != 10u || a[1] != 14u || a[2] != 15u || a[3] != 16u) fails |= 2;
    if (a[7] != 0xDEADu) fails |= 4;                   // capacity beyond finish untouched
    r = Erase(v, a + 2, a + 2);
    if (r != a + 2 || v.f != a + 4 || a[2] != 15u || a[3] != 16u) fails |= 8;
    r = Erase(v, a + 3, v.f);
    if (r != a + 3 || v.f != a + 3) fails |= 16;
    r = Erase(v, v.s, v.f);
    if (r != a || v.f != a) fails |= 32;
    if (fails) { std::printf("0046988f_TEST FAIL %d\n", fails); return 1; }
    std::printf("0046988f_TEST PASS\n");
    return 0;
}