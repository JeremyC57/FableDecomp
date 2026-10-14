#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_freedElements = 0; static void* g_freedBlock = 0;
static void FreeElements(void** first, void** last) { for (; first < last; ++first) if (*first) { ++g_freedElements; } }
static void BlockFree(void* p) { g_freedBlock = p; }
struct CPointerArrayOwner {
    void** m_pElements; void** m_pElementsEnd; void* m_pBlock;
    void FreeBuffer()
    {
        if (m_pBlock != 0) {
            FreeElements(m_pElements, m_pElementsEnd + 1);
            if (m_pBlock != 0) BlockFree(m_pBlock);
        }
    }
};
static bool Check()
{
    int a, b, c; void* elems[4] = { &a, 0, &b, &c };
    CPointerArrayOwner o; o.m_pElements = elems; o.m_pElementsEnd = elems + 3; o.m_pBlock = 0;
    o.FreeBuffer(); if (g_freedElements != 0 || g_freedBlock != 0) return false;      // no block: nothing happens
    o.m_pBlock = elems; o.FreeBuffer();
    return g_freedElements == 3 && g_freedBlock == elems;                               // the range is one past the end pointer
}
int main()
{
    if (!Check()) { std::printf("0059ac77_TEST FAIL\n"); return 1; }
    std::printf("0059ac77_TEST PASS\n");
    return 0;
}
