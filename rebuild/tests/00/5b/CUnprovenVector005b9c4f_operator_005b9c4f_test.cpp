// test for 0x005B9C4F CUnprovenVector_005b9c4f::operator[](unsigned int index)
#include <stdio.h>
struct CUnprovenElement12_005b9c4f { long Words[3]; };
class CUnprovenVector_005b9c4f {
public:
    CUnprovenElement12_005b9c4f& operator[](unsigned int index);
    CUnprovenElement12_005b9c4f* Start;
    CUnprovenElement12_005b9c4f* Finish;
    CUnprovenElement12_005b9c4f* EndOfStorage;
};

CUnprovenElement12_005b9c4f& CUnprovenVector_005b9c4f::operator[](unsigned int index)
{
    return Start[index];
}
static long g_cell, g_cell2;
static int run()
{
    CUnprovenElement12_005b9c4f items[3]; CUnprovenVector_005b9c4f v; v.Start = items; v.Finish = items + 3; v.EndOfStorage = items + 3;
    if (&v[2] != &items[2] || &v[0] != items) return 1;
    return 0;
}
int main()
{
    (void)g_cell2;
    if (run() != 0) { printf("005b9c4f_TEST FAIL\n"); return 1; }
    printf("005b9c4f_TEST PASS\n");
    return 0;
}