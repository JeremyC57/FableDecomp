// test for 0x005B9CAE CUnprovenVector_005b9cae::operator[](unsigned int index)
#include <stdio.h>
struct CUnprovenElement4_005b9cae { long Words[1]; };
class CUnprovenVector_005b9cae {
public:
    CUnprovenElement4_005b9cae& operator[](unsigned int index);
    CUnprovenElement4_005b9cae* Start;
    CUnprovenElement4_005b9cae* Finish;
    CUnprovenElement4_005b9cae* EndOfStorage;
};

CUnprovenElement4_005b9cae& CUnprovenVector_005b9cae::operator[](unsigned int index)
{
    return Start[index];
}
static long g_cell, g_cell2;
static int run()
{
    CUnprovenElement4_005b9cae items[3]; CUnprovenVector_005b9cae v; v.Start = items; v.Finish = items + 3; v.EndOfStorage = items + 3;
    if (&v[2] != &items[2] || &v[0] != items) return 1;
    return 0;
}
int main()
{
    (void)g_cell2;
    if (run() != 0) { printf("005b9cae_TEST FAIL\n"); return 1; }
    printf("005b9cae_TEST PASS\n");
    return 0;
}