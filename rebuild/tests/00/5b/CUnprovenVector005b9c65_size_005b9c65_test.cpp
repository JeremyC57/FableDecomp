// test for 0x005B9C65 CUnprovenVector_005b9c65::size() const
#include <stdio.h>
struct CUnprovenElement24_005b9c65 { long Words[6]; };
class CUnprovenVector_005b9c65 {
public:
    unsigned int size() const;
    CUnprovenElement24_005b9c65* Start;
    CUnprovenElement24_005b9c65* Finish;
    CUnprovenElement24_005b9c65* EndOfStorage;
};

unsigned int CUnprovenVector_005b9c65::size() const
{
    return Finish - Start;
}
static long g_cell, g_cell2;
static int run()
{
    CUnprovenElement24_005b9c65 items[5]; CUnprovenVector_005b9c65 v; v.Start = items; v.Finish = items + 4; v.EndOfStorage = items + 5;
    if (v.size() != 4) return 1;
    return 0;
}
int main()
{
    (void)g_cell2;
    if (run() != 0) { printf("005b9c65_TEST FAIL\n"); return 1; }
    printf("005b9c65_TEST PASS\n");
    return 0;
}