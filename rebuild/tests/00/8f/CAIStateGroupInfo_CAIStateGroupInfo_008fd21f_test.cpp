// Behaviour test for 0x008FD21F CAIStateGroupInfo::CAIStateGroupInfo(CCharString, alloc): copies the name, stores the allocator.
#include <cstdio>
#include <cstring>

static int g_copies = 0;
static int g_dtors = 0;

struct CCharString {
    const char* PStringData;
    explicit CCharString(const char* s) : PStringData(s) { }
    CCharString(const CCharString& other) : PStringData(other.PStringData) { ++g_copies; }
    ~CCharString() { ++g_dtors; }
};

struct CAIStateGroupBase { int kind; };
typedef CAIStateGroupBase* (*TAIStateGroupAllocFunc)();

static CAIStateGroupBase g_wander = { 42 };
static CAIStateGroupBase* AllocWander() { return &g_wander; }

struct CAIStateGroupInfo {
    CCharString            StateGroupName;
    TAIStateGroupAllocFunc AllocFunc;
    CAIStateGroupInfo(CCharString name, TAIStateGroupAllocFunc alloc);
};

CAIStateGroupInfo::CAIStateGroupInfo(CCharString name, TAIStateGroupAllocFunc alloc)
    : StateGroupName(name)
    , AllocFunc(alloc)
{
}

int main()
{
    int fail = 0;
    {
        CCharString literal("SG_MINION_WANDER");
        CAIStateGroupInfo info(literal, AllocWander);
        if (std::strcmp(info.StateGroupName.PStringData, "SG_MINION_WANDER") != 0) fail = 1;
        if (info.AllocFunc != AllocWander || info.AllocFunc()->kind != 42) fail = 1;
        if (g_copies != 2 || g_dtors != 1) fail = 1;    // argument copy + member copy; argument destroyed by the callee
    }
    if (g_dtors != 3) fail = 1;
    if (fail) { std::printf("008fd21f_TEST FAIL\n"); return 1; }
    std::printf("008fd21f_TEST PASS\n");
    return 0;
}