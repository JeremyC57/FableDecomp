// behaviour test for 0x004B9620 vector<CUnprovenElement_004b9620 >(const vector& x): an empty x gives three null pointers; otherwise exactly x.size() slots are allocated, the elements are copied bit-for-bit and finish == end of storage
#include <cstdio>
#include <cstdlib>
#include <cstring>
struct Vec { unsigned int* s; unsigned int* f; unsigned int* e; };
static Vec Copy(const Vec& x)
{
    Vec v; v.s = 0; v.f = 0; v.e = 0;
    unsigned int n = (unsigned int)(x.f - x.s);
    unsigned int* p = n != 0 ? (unsigned int*)std::malloc(n * 4) : 0;
    v.s = p; v.f = p; v.e = p + n;
    if (x.f != x.s) { std::memmove(p, x.s, (char*)x.f - (char*)x.s); p += n; }
    v.f = p;
    return v;
}
int main()
{
    int fails = 0;
    if ((unsigned int)sizeof(unsigned int) != 4u) fails |= 1;
    Vec empty = { 0, 0, 0 };
    Vec a = Copy(empty);
    if (a.s != 0 || a.f != 0 || a.e != 0) fails |= 2;
    unsigned int src[5] = { 1u, 0xFFFFFFFFu, 0x80000000u, 7u, 0u };
    Vec x = { src, src + 5, src + 8 };                 // capacity beyond size is not copied
    Vec b = Copy(x);
    if (b.s == 0 || b.f != b.s + 5 || b.e != b.s + 5) fails |= 4;
    for (int i = 0; i < 5; ++i) if (b.s[i] != src[i]) fails |= 8;
    src[0] = 99u;
    if (b.s[0] != 1u) fails |= 16;                      // a separate block
    std::free(b.s);
    if (fails) { std::printf("004b9620_TEST FAIL %d\n", fails); return 1; }
    std::printf("004b9620_TEST PASS\n");
    return 0;
}