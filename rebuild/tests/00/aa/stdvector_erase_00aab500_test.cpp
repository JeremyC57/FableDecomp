// behaviour test for 0x00AAB500 vector<CPlane >::erase(first, last): the tail [last, finish) is copied down onto first as
// four-dword records, finish moves back by last - first, first is returned; an empty range leaves the vector alone
#include <cstdio>
struct Rec { unsigned int a, b, c, d; };                    // the 16-byte trivially destroyed element
struct Vec { Rec* s; Rec* f; Rec* e; };
static Rec* Erase(Vec& v, Rec* first, Rec* last)
{
    Rec* result = first;
    for (int n = (int)(v.f - last); n > 0; --n) { *result = *last; ++last; ++result; }
    v.f = result;
    return first;
}
int main()
{
    int fails = 0;
    if (sizeof(Rec) != 16) fails |= 1;
    Rec buf[8];
    for (unsigned int k = 0; k < 8; ++k) { buf[k].a = k; buf[k].b = k * 2; buf[k].c = k * 3; buf[k].d = k * 4; }
    Vec v; v.s = buf; v.f = buf + 6; v.e = buf + 8;
    Rec* r = Erase(v, buf + 1, buf + 3);                     // drop records 1, 2
    if (r != buf + 1 || v.f != buf + 4) fails |= 2;
    if (buf[1].a != 3 || buf[2].a != 4 || buf[3].a != 5 || buf[3].d != 20) fails |= 4;
    r = Erase(v, buf + 2, buf + 2);                          // empty range
    if (r != buf + 2 || v.f != buf + 4 || buf[2].a != 4) fails |= 8;
    r = Erase(v, v.s, v.f);                                  // clear
    if (r != buf || v.f != buf) fails |= 16;
    if (fails) { std::printf("00aab500_TEST FAIL %d\n", fails); return 1; }
    std::printf("00aab500_TEST PASS\n");
    return 0;
}