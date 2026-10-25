// behaviour test for 0x0065ED59 CVillageTaskTorch::~CVillageTaskTorch(): the two members are destroyed in reverse declaration order (the member at +0x10 first), then the base part (CVillageTaskBase, whose destructor resets the vptr to its own table) and finally CBaseObject, exactly once each
#include <cstdio>
static char gLog[8];
static int gN = 0;
struct BB { virtual ~BB() { gLog[gN++] = 'R'; } };
struct B : BB { virtual ~B() { gLog[gN++] = 'B'; } };
struct M1 { ~M1() { gLog[gN++] = '1'; } };
struct M2 { ~M2() { gLog[gN++] = '2'; } };
struct C : B { M1 m1; M2 m2; };              // implicit destructor, like CVillageTaskTorch
int main()
{
    int fails = 0;
    { C c; }
    if (gN != 4 || gLog[0] != '2' || gLog[1] != '1' || gLog[2] != 'B' || gLog[3] != 'R') fails |= 1;
    gN = 0;
    BB* p = new C;
    delete p;                                  // through the virtual destructor
    if (gN != 4 || gLog[0] != '2' || gLog[3] != 'R') fails |= 2;
    if (fails) { std::printf("0065ed59_TEST FAIL %d\n", fails); return 1; }
    std::printf("0065ed59_TEST PASS\n");
    return 0;
}