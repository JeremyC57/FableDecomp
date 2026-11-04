// Behaviour test for NUISystem::CDraggableInto::~CDraggableInto (0x00558fd0): a model of the source's logic with its out-of-line callees stubbed.
#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_fail = 0;
static void Check(bool ok, int line) { if (!ok) { g_fail = 1; std::printf("check failed at line %d\n", line); } }
#define CHECK(c) Check((c) ? true : false, __LINE__)
static int g_clearCalls = 0, g_freeCalls = 0, g_baseDtors = 0, g_secondDtors = 0;
struct CActionList {
    void* _M_node;
    CActionList() : _M_node(std::malloc(12)) { }
    void clear() { ++g_clearCalls; }                               // stub for 0x005356A0
    ~CActionList() { clear(); if (_M_node != 0) { std::free(_M_node); ++g_freeCalls; } }
};
struct CComponentRoot { virtual ~CComponentRoot() { } };
struct CGuiControl { virtual ~CGuiControl() { ++g_secondDtors; } unsigned char _body[0x10]; };
struct CHoverableBase { virtual ~CHoverableBase() { } };
struct CHoverable : CComponentRoot, CGuiControl, CHoverableBase {
    virtual ~CHoverable() { ++g_baseDtors; }                      // stub for 0x0055BC30
};
struct CDraggableInto : CHoverable {
    CActionList* PActions;
    CDraggableInto(CActionList* a) : PActions(a) { }
    virtual ~CDraggableInto() { delete PActions; }
};
static void RunTest()
{
    { CDraggableInto d(new CActionList); }                       // the plain destructor (automatic object)
    CHECK(g_clearCalls == 1 && g_freeCalls == 1 && g_baseDtors == 1);
    { CDraggableInto d(0); }                                     // no owned list
    CHECK(g_clearCalls == 1 && g_baseDtors == 2);
}
int main()
{
    RunTest();
    if (g_fail) { std::printf("00558fd0_TEST FAIL\n"); return 1; }
    std::printf("00558fd0_TEST PASS\n");
    return 0;
}