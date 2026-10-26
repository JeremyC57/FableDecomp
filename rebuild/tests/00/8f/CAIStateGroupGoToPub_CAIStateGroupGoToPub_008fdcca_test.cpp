// behaviour test for 0x008fdcca CAIStateGroup_GoToPub::~CAIStateGroup_GoToPub() (implicit): the class-typed members are destroyed in reverse declaration
// order, plain members are left alone, and the base destructor runs last
#include <cstdio>
static int gLog[16]; static int gN = 0;
struct Member { int Id; Member() : Id(0) { } ~Member() { gLog[gN++] = Id; } };
struct Base { virtual ~Base() { gLog[gN++] = 100; } };
struct Obj : public Base {
    Member M1;
    Member M2;
    Member M3;
    Member M4;
    Member M5;
};
int main()
{
    int fail = 0;
    {
        Obj o;
        o.M1.Id = 1;
        o.M2.Id = 2;
        o.M3.Id = 3;
        o.M4.Id = 4;
        o.M5.Id = 5;
    }
    int want[16]; int n = 0;
    want[n++] = 5;
    want[n++] = 4;
    want[n++] = 3;
    want[n++] = 2;
    want[n++] = 1;
    want[n++] = 100;
    if (gN != n) fail |= 1;
    for (int k = 0; k < n && k < gN; ++k) if (gLog[k] != want[k]) fail |= 2;
    if (fail) { std::printf("008fdcca_TEST FAIL %d\n", fail); return 1; }
    std::printf("008fdcca_TEST PASS\n");
    return 0;
}