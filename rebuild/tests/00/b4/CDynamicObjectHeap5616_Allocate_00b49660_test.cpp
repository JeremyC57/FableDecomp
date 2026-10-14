#include <cstdio>
struct SFreeNode { SFreeNode* next; };
static SFreeNode g_nodes[3]; static int g_grows = 0;
struct CDynamicObjectHeap_56_16 { void* __vftable; unsigned long m_allocated; unsigned char _pad0[0x0c]; SFreeNode* m_pFreeList; unsigned char _pad1[0x384]; bool m_bCanGrow; void CreateMoreBlocks(); void* Allocate(unsigned long size); };
void CDynamicObjectHeap_56_16::CreateMoreBlocks() { ++g_grows; g_nodes[0].next = &g_nodes[1]; g_nodes[1].next = &g_nodes[2]; g_nodes[2].next = 0; m_pFreeList = &g_nodes[0]; }
void* CDynamicObjectHeap_56_16::Allocate(unsigned long size)
{
    if (m_pFreeList == 0) { if (m_bCanGrow) CreateMoreBlocks(); else return 0; }
    ++m_allocated;
    SFreeNode* node = m_pFreeList; m_pFreeList = node->next; return node;
}
int main()
{
    CDynamicObjectHeap_56_16 h; h.m_allocated = 0; h.m_pFreeList = 0; h.m_bCanGrow = false;
    if (h.Allocate(56) != 0 || h.m_allocated != 0) { std::printf("00b49660_TEST FAIL\n"); return 1; }
    h.m_bCanGrow = true;
    if (h.Allocate(56) != &g_nodes[0] || g_grows != 1 || h.m_allocated != 1 || h.m_pFreeList != &g_nodes[1]) { std::printf("00b49660_TEST FAIL\n"); return 1; }
    if (h.Allocate(56) != &g_nodes[1] || g_grows != 1 || h.m_allocated != 2) { std::printf("00b49660_TEST FAIL\n"); return 1; }
    std::printf("00b49660_TEST PASS\n");
    return 0;
}