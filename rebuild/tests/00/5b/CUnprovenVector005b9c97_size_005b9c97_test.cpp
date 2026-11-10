// test for 0x005B9C97 CUnprovenVector_005b9c97::size() const
#include <stdio.h>
struct CUnprovenElement12_005b9c97 { long Words[3]; };
class CUnprovenVector_005b9c97 {
public:
    unsigned int size() const;
    CUnprovenElement12_005b9c97* Start;
    CUnprovenElement12_005b9c97* Finish;
    CUnprovenElement12_005b9c97* EndOfStorage;
};

unsigned int CUnprovenVector_005b9c97::size() const
{
    return Finish - Start;
}
static long g_cell, g_cell2;
static int run()
{
    CUnprovenElement12_005b9c97 items[5]; CUnprovenVector_005b9c97 v; v.Start = items; v.Finish = items + 4; v.EndOfStorage = items + 5;
    if (v.size() != 4) return 1;
    return 0;
}
int main()
{
    (void)g_cell2;
    if (run() != 0) { printf("005b9c97_TEST FAIL\n"); return 1; }
    printf("005b9c97_TEST PASS\n");
    return 0;
}