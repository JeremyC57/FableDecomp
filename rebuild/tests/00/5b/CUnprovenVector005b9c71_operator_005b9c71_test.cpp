// test for 0x005B9C71 CUnprovenVector_005b9c71::operator[](unsigned int index)
#include <stdio.h>
struct CUnprovenElement24_005b9c71 { long Words[6]; };
class CUnprovenVector_005b9c71 {
public:
    CUnprovenElement24_005b9c71& operator[](unsigned int index);
    CUnprovenElement24_005b9c71* Start;
    CUnprovenElement24_005b9c71* Finish;
    CUnprovenElement24_005b9c71* EndOfStorage;
};

CUnprovenElement24_005b9c71& CUnprovenVector_005b9c71::operator[](unsigned int index)
{
    return Start[index];
}
static long g_cell, g_cell2;
static int run()
{
    CUnprovenElement24_005b9c71 items[3]; CUnprovenVector_005b9c71 v; v.Start = items; v.Finish = items + 3; v.EndOfStorage = items + 3;
    if (&v[2] != &items[2] || &v[0] != items) return 1;
    return 0;
}
int main()
{
    (void)g_cell2;
    if (run() != 0) { printf("005b9c71_TEST FAIL\n"); return 1; }
    printf("005b9c71_TEST PASS\n");
    return 0;
}