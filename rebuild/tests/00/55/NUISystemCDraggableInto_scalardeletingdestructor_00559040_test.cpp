// Behaviour test for 00559040: a copy of the source's model and logic with its out-of-line callees stubbed.
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

struct CHoverable {
    virtual ~CHoverable() { ++g_baseDtors; }                       // stub for 0x0055BC30
};
struct CDraggableInto : CHoverable {
    CActionList* PActions;
    CDraggableInto(CActionList* a) : PActions(a) { }
    virtual ~CDraggableInto() { delete PActions; }
};
static void RunTest()
{
    CHoverable* p = new CDraggableInto(new CActionList);
    delete p;                                                      // the scalar deleting destructor through the vtable
    CHECK(g_clearCalls == 1 && g_freeCalls == 1 && g_baseDtors == 1);
    p = new CDraggableInto(0);                                     // no owned list
    delete p;
    CHECK(g_clearCalls == 1 && g_baseDtors == 2);
}

int main()
{
    RunTest();
    if (g_fail) {
        std::printf("00559040_TEST FAIL\n");
        return 1;
    }
    std::printf("00559040_TEST PASS\n");
    return 0;
}