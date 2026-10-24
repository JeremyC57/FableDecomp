// Behaviour test for 0052ada0: a copy of the source's model and logic with its out-of-line callees stubbed.
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


static int g_step = 0, g_baseStep = 0, g_initStep = 0;
static long g_baseWorkspace = 0;
struct CACompressor {
    long WorkspaceSize;
    CACompressor(long workspaceSize) : WorkspaceSize(workspaceSize)    // stub for 0x00A3AEA0
    {
        g_baseWorkspace = workspaceSize;
        g_baseStep = ++g_step;
    }
    virtual ~CACompressor() { }
    virtual int Kind() const { return 0; }
};
class CCompressorZlib : public CACompressor {
public:
    unsigned char Stream[0x38];
    bool ZlibInitialised;
    bool Compressing;
    bool ErrorFlag;
    static long GetWorkspaceSize() { return 0x10840; }               // stub for 0x00A3A7D0
    void Init()                                                      // stub for 0x00A3AA10
    {
        g_initStep = ++g_step;
        ZlibInitialised = false;
        Compressing = false;
        ErrorFlag = false;
        CHECK(Kind() == 1);                                          // the derived vtable is already in place
    }
    virtual int Kind() const { return 1; }
    CCompressorZlib() : CACompressor(GetWorkspaceSize()) { Init(); }
};
static void RunTest()
{
    CCompressorZlib z;
    CHECK(g_baseWorkspace == 0x10840 && z.WorkspaceSize == 0x10840);
    CHECK(g_baseStep == 1 && g_initStep == 2);                       // base first, then Init
    CHECK(!z.ZlibInitialised && !z.Compressing && !z.ErrorFlag && z.Kind() == 1);
}

int main()
{
    RunTest();
    if (g_fail) {
        std::printf("0052ada0_TEST FAIL\n");
        return 1;
    }
    std::printf("0052ada0_TEST PASS\n");
    return 0;
}