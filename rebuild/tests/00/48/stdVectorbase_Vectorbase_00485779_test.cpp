// behaviour test for 0x00485779 _Vector_base<CEnginePrimitiveHandle >(n, a): n == 0 leaves three null pointers; n > 0 allocates n elements with finish == start and end == start + n
#include <cstdio>
#include <cstdlib>
struct Elem { unsigned char b[8]; };
struct Base {
    Elem* s; Elem* f; Elem* e;
    Base(unsigned int n) : s(0), f(0), e(0)
    {
        s = n != 0 ? (Elem*)std::malloc(n * sizeof(Elem)) : 0;
        f = s;
        e = s + n;
    }
};
int main()
{
    int fails = 0;
    if ((unsigned int)sizeof(Elem) != 8u) fails |= 1;
    Base a(0);
    if (a.s != 0 || a.f != 0 || a.e != 0) fails |= 2;
    Base b(5);
    if (b.s == 0 || b.f != b.s || b.e - b.s != 5 || (char*)b.e - (char*)b.s != 5 * 8) fails |= 4;
    for (int i = 0; i < 5 * 8; ++i) ((unsigned char*)b.s)[i] = (unsigned char)i;
    std::free(b.s);
    if (fails) { std::printf("00485779_TEST FAIL %d\n", fails); return 1; }
    std::printf("00485779_TEST PASS\n");
    return 0;
}