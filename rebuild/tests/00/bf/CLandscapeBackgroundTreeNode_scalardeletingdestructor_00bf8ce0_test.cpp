// behaviour test for 0x00BF8CE0 CLandscapeBackgroundTreeNode's scalar deleting destructor (model: the destructor runs, then the manager frees the block)
#include <cstdio>
#include <cstdlib>
static int g_dtor = 0; static void* g_freed = 0;
struct Manager { void FreeVirtual(void* b) { g_freed = b; std::free(b); } };
static Manager g_mgr;
class CLandscapeBackgroundTreeNode { public:
    virtual ~CLandscapeBackgroundTreeNode() { ++g_dtor; }
    static void* operator new(size_t n) { return std::malloc(n); }
    static void operator delete(void* b) { g_mgr.FreeVirtual(b); }
};
int main()
{
    CLandscapeBackgroundTreeNode* p = new CLandscapeBackgroundTreeNode;
    void* raw = p;
    delete p;
    int ok = g_dtor == 1 && g_freed == raw;
    if (!ok) { std::printf("00bf8ce0_TEST FAIL\n"); return 1; }
    std::printf("00bf8ce0_TEST PASS\n");
    return 0;
}