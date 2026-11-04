// Behaviour test for CAIStateGroup_WorkHammerAnvil::CAIStateGroup_WorkHammerAnvil (0x008ffdce): a model of the source's logic with its out-of-line callees stubbed.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstddef>
static int g_fail = 0;
static void Check(bool ok, int line) { if (!ok) { g_fail = 1; std::printf("check failed at line %d\n", line); } }
#define CHECK(c) Check((c) ? true : false, __LINE__)
static int g_baseCtors = 0, g_pointerCtors = 0;
struct CAIStateGroupBase {
    CAIStateGroupBase() { ++g_baseCtors; }                         // stub for 0x008FD255
    virtual ~CAIStateGroupBase() { }
    unsigned char _body[0x10];
};
#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    void* PData;
    CBaseIntelligentPointer() : PData(0) { ++g_pointerCtors; }      // stub for 0x00A01B10
    virtual ~CBaseIntelligentPointer() { }
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer { CIntelligentPointer() { } };
#pragma pack(pop)
struct CThing;
struct CAIStateGroup_WorkHammerAnvil : CAIStateGroupBase {
    bool TookActionPoint;
    CIntelligentPointer<CThing> PAnvil;
    CAIStateGroup_WorkHammerAnvil() { }
};
static void RunTest()
{
    CAIStateGroup_WorkHammerAnvil g;
    CHECK(g_baseCtors == 1 && g_pointerCtors == 1);               // base first, then the member pointer
    CHECK(g.PAnvil.PData == 0);
    CHECK((char*)&g.PAnvil - (char*)&g.TookActionPoint == 1);          // the packed pointer follows the bool directly
}
int main()
{
    RunTest();
    if (g_fail) { std::printf("008ffdce_TEST FAIL\n"); return 1; }
    std::printf("008ffdce_TEST PASS\n");
    return 0;
}