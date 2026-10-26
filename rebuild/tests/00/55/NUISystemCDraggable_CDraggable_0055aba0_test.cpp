// Behaviour test for 0055aba0: a copy of the source's model and logic with its out-of-line callees stubbed.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
static int g_fail = 0;
static void Check(bool ok, int line)
{
    if (!ok) {
        g_fail = 1;
        std::printf("check failed at line %d\n", line);
    }
}
#define CHECK(c) Check((c) ? true : false, __LINE__)

static int g_clearCalls = 0, g_freeCalls = 0, g_baseDtors = 0;
struct CActionList {
    void* _M_node;
    CActionList() : _M_node(std::malloc(12)) { }
    void clear() { ++g_clearCalls; }                               // stub for 0x005356A0
    ~CActionList()
    {
        clear();
        if (_M_node != 0) {
            std::free(_M_node);
            ++g_freeCalls;
        }
    }
};

class C2DVector { public: float X, Y; };
static int g_order = 0, g_baseOrder = 0;
struct CClickable {
    virtual ~CClickable() { ++g_baseDtors; g_baseOrder = ++g_order; }   // stub for 0x0055B760 (the tail jump)
};
struct CDraggable : CClickable {
    C2DVector    PositionBeforeDrag;
    bool         WasDragged;
    CActionList* PActionsOnDropped;
    CActionList* PActionsOnDroppedNowhere;
    bool         SnapBackIfDroppedNowhere;
    bool         ChangeLayer;
    CDraggable(CActionList* a, CActionList* b) : PActionsOnDropped(a), PActionsOnDroppedNowhere(b) { }
    virtual ~CDraggable()
    {
        delete PActionsOnDropped;
        delete PActionsOnDroppedNowhere;
        g_order = 10;
    }
};
static void RunTest()
{
    {
        CDraggable d(new CActionList, new CActionList);
    }
    CHECK(g_clearCalls == 2 && g_freeCalls == 2 && g_baseDtors == 1 && g_baseOrder == 11);   // base last
    {
        CDraggable d(0, new CActionList);
    }
    CHECK(g_clearCalls == 3 && g_baseDtors == 2);
}

int main()
{
    RunTest();
    if (g_fail) {
        std::printf("0055aba0_TEST FAIL\n");
        return 1;
    }
    std::printf("0055aba0_TEST PASS\n");
    return 0;
}