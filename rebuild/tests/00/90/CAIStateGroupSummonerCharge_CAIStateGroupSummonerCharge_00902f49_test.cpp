// behaviour test for 0x00902f49 CAIStateGroup_SummonerCharge::~CAIStateGroup_SummonerCharge() (implicit): the def pointer is released first (its count drops by one, the def is
// deleted only when the count reaches zero, a null pointer is left alone), then PTarget, then the base destructor
#include <cstdio>
static int gLog[16]; static int gN = 0;
struct Pointee { unsigned int Refs; int Deleted; };
struct Member { int Id; Member() : Id(0) { } ~Member() { gLog[gN++] = Id; } };
struct DefPointer {
    Pointee* Object;
    DefPointer() : Object(0) { }
    ~DefPointer() { gLog[gN++] = 50; Pointee* p = Object; if (p) { if (--p->Refs == 0) p->Deleted++; Object = 0; } }
};
struct Base { virtual ~Base() { gLog[gN++] = 100; } };
struct Obj : public Base {
    Member M2;
    DefPointer PDef;
    int Plain;
};
static int Run(Pointee* def)
{
    gN = 0;
    {
        Obj o;
        o.M2.Id = 2; o.Plain = 7;
        o.PDef.Object = def;
    }
    int want[4]; int n = 0;
    want[n++] = 50;
    want[n++] = 2;
    want[n++] = 100;
    if (gN != n) return 1;
    for (int k = 0; k < n; ++k) if (gLog[k] != want[k]) return 2;
    return 0;
}
int main()
{
    int fail = 0;
    Pointee d; d.Refs = 2; d.Deleted = 0;
    if (Run(&d) != 0) fail |= 1;
    if (d.Refs != 1 || d.Deleted != 0) fail |= 2;
    if (Run(&d) != 0) fail |= 4;
    if (d.Refs != 0 || d.Deleted != 1) fail |= 8;
    if (Run(0) != 0) fail |= 16;
    if (fail) { std::printf("00902f49_TEST FAIL %d\n", fail); return 1; }
    std::printf("00902f49_TEST PASS\n");
    return 0;
}