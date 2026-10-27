// behaviour test for 0x004a96a0 vector<8-byte element>::operator[](n): reference to *(start + n)
#include <cstdio>
struct E { int a, b; };
struct Vec { E* start; E* finish; E* eos; E& operator[](unsigned int n) { return *(start + n); } };
int main()
{
    int fails = 0;
    E d[4] = { { 0, 1 }, { 2, 3 }, { 4, 5 }, { 6, 7 } };
    Vec v; v.start = d; v.finish = v.eos = d + 4;
    if (&v[0] != d || &v[3] != d + 3 || v[2].b != 5) fails |= 1;
    v[1].a = 99;
    if (d[1].a != 99) fails |= 2;
    if (fails) { std::printf("004a96a0_TEST FAIL %d\n", fails); return 1; }
    std::printf("004a96a0_TEST PASS\n");
    return 0;
}