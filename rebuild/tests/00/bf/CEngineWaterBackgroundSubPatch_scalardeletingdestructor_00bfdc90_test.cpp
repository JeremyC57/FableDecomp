// behaviour test for 0x00BFDC90 CEngineWaterBackgroundSubPatch's scalar deleting destructor (model: the destructor runs, then the manager frees the block)
#include <cstdio>
#include <cstdlib>
static int g_dtor = 0; static void* g_freed = 0;
struct Manager { void FreeVirtual(void* b) { g_freed = b; std::free(b); } };
static Manager g_mgr;
class CEngineWaterBackgroundSubPatch { public:
    virtual ~CEngineWaterBackgroundSubPatch() { ++g_dtor; }
    static void* operator new(size_t n) { return std::malloc(n); }
    static void operator delete(void* b) { g_mgr.FreeVirtual(b); }
};
int main()
{
    CEngineWaterBackgroundSubPatch* p = new CEngineWaterBackgroundSubPatch;
    void* raw = p;
    delete p;
    int ok = g_dtor == 1 && g_freed == raw;
    if (!ok) { std::printf("00bfdc90_TEST FAIL\n"); return 1; }
    std::printf("00bfdc90_TEST PASS\n");
    return 0;
}