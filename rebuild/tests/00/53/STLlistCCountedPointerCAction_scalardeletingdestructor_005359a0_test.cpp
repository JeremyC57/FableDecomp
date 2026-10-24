// Behaviour test for 005359a0: a copy of the source's model and logic with its out-of-line callees stubbed.
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

static int g_clearCalls = 0;
static void* g_freed = 0;
static void TrackedFree(void* p) { g_freed = p; std::free(p); }   // the CRT free 0x00BFEA14
struct CActionList {
    void* _M_node;
    int Cleared;
    CActionList() : _M_node(std::malloc(12)), Cleared(0) { }
    void clear() { ++g_clearCalls; ++Cleared; }                    // stub for 0x005356A0
    ~CActionList()
    {
        clear();
        if (_M_node != 0)
            TrackedFree(_M_node);
    }
};
static void DeleteActionList(CActionList* list) { delete list; }
static void RunTest()
{
    CActionList* list = new CActionList;
    void* sentinel = list->_M_node;
    DeleteActionList(list);
    CHECK(g_clearCalls == 1 && g_freed == sentinel);
    CActionList* empty = new CActionList;
    std::free(empty->_M_node);
    empty->_M_node = 0;                                            // no sentinel: only the clear
    g_freed = 0;
    DeleteActionList(empty);
    CHECK(g_clearCalls == 2 && g_freed == 0);
    DeleteActionList(0);                                           // delete of null does nothing
    CHECK(g_clearCalls == 2);
}

int main()
{
    RunTest();
    if (g_fail) {
        std::printf("005359a0_TEST FAIL\n");
        return 1;
    }
    std::printf("005359a0_TEST PASS\n");
    return 0;
}