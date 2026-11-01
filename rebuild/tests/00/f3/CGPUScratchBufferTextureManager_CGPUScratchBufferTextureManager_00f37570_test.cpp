// Behaviour test: CGPUScratchBufferTextureManager::~CGPUScratchBufferTextureManager (0x00F37570) -- every memory page
// still bound to a scratch pool (ScratchPool > -1) is freed through FreeMemoryPage(i), which unbinds it.
#include <cstdio>
struct CResourcePage { long ScratchPool; };
static long g_freed[8]; static int g_n = 0;
class CGPUScratchBufferTextureManager { public:
    CResourcePage pages[5]; unsigned int count;
    unsigned int size() const { return count; }
    void FreeMemoryPage(long index) { g_freed[g_n++] = index; pages[index].ScratchPool = -1; }
    ~CGPUScratchBufferTextureManager()
    {
        for (long i = 0; i < (long)size(); ++i) {
            if (pages[i].ScratchPool > -1)
                FreeMemoryPage(i);
        }
    }
};
int main()
{
    {
        CGPUScratchBufferTextureManager m;
        m.count = 5;
        m.pages[0].ScratchPool = 0; m.pages[1].ScratchPool = -1; m.pages[2].ScratchPool = 3;
        m.pages[3].ScratchPool = -7; m.pages[4].ScratchPool = 1;
    }
    if (g_n != 3 || g_freed[0] != 0 || g_freed[1] != 2 || g_freed[2] != 4) { std::printf("00f37570_TEST FAIL\n"); return 1; }
    { CGPUScratchBufferTextureManager e; e.count = 0; }
    if (g_n != 3) { std::printf("00f37570_TEST FAIL\n"); return 1; }
    std::printf("00f37570_TEST PASS\n");
    return 0;
}