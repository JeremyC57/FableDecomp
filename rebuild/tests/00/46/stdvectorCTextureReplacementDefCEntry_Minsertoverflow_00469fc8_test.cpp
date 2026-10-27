// behaviour test for 0x00469FC8 vector<CTextureReplacementDef::CEntry>::_M_insert_overflow (model: head, n copies, tail into a grown block)
#include <cstdio>
#include <cstdlib>
struct E { int a, b; };
int main()
{
    E old[3] = { { 1, 1 }, { 2, 2 }, { 3, 3 } };
    E x = { 9, 9 };
    unsigned int oldSize = 3, n = 2, len = oldSize + (oldSize > n ? oldSize : n);
    E* nb = (E*)std::malloc(len * sizeof(E)); E* f = nb;
    for (E* p = old; p != old + 1; ++p) *f++ = *p;
    for (unsigned int i = 0; i < n; ++i) *f++ = x;
    for (E* p = old + 1; p != old + 3; ++p) *f++ = *p;
    int ok = len == 6 && f - nb == 5 && nb[1].a == 9 && nb[2].b == 9 && nb[3].a == 2 && nb[4].a == 3;
    std::free(nb);
    if (!ok) { std::printf("00469fc8_TEST FAIL\n"); return 1; }
    std::printf("00469fc8_TEST PASS\n");
    return 0;
}