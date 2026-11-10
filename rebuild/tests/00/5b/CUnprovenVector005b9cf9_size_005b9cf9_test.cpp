// test for 0x005B9CF9 CUnprovenVector_005b9cf9::size() const
#include <stdio.h>
struct CUnprovenElement20_005b9cf9 { long Words[5]; };
class CUnprovenVector_005b9cf9 {
public:
    unsigned int size() const;
    CUnprovenElement20_005b9cf9* Start;
    CUnprovenElement20_005b9cf9* Finish;
    CUnprovenElement20_005b9cf9* EndOfStorage;
};

unsigned int CUnprovenVector_005b9cf9::size() const
{
    return Finish - Start;
}
static long g_cell, g_cell2;
static int run()
{
    CUnprovenElement20_005b9cf9 items[5]; CUnprovenVector_005b9cf9 v; v.Start = items; v.Finish = items + 4; v.EndOfStorage = items + 5;
    if (v.size() != 4) return 1;
    return 0;
}
int main()
{
    (void)g_cell2;
    if (run() != 0) { printf("005b9cf9_TEST FAIL\n"); return 1; }
    printf("005b9cf9_TEST PASS\n");
    return 0;
}