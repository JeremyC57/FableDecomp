// Behaviour test for 00c1b1e0: CMapNodeAllocator::Release -- frees the node through the virtual slot and clears the pointer
#include <cstdio>
struct PKCTRRedblackNode { long a; };
static void* g_freed = 0;
struct CMapNodeAllocator { virtual void Free(void* node) { g_freed = node; } virtual ~CMapNodeAllocator() { }
    void Release(PKCTRRedblackNode** node) { Free(*node); *node = 0; } };
int main()
{
    CMapNodeAllocator a; PKCTRRedblackNode n; PKCTRRedblackNode* p = &n;
    a.Release(&p);
    if (g_freed != &n || p != 0) { std::printf("00c1b1e0_TEST FAIL\n"); return 1; }
    std::printf("00c1b1e0_TEST PASS\n");
    return 0;
}
