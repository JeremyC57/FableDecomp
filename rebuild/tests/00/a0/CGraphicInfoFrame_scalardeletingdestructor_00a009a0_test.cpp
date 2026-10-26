#include <cstdio>
// Behaviour test for CGraphicInfoFrame scalar deleting destructor (0x00A009A0): shared block released on last ref, members destroyed in reverse order, base last.
static int g_order[8]; static int g_n = 0; static int g_destroyed = 0;
struct SSharedBlock {
    long m_references;
    void Destroy() { ++g_destroyed; }
    void Release() { if (--m_references == 0) Destroy(); }
};
struct CSharedPointer {
    SSharedBlock* m_pBlock;
    ~CSharedPointer() { g_order[g_n++] = 2; if (m_pBlock) { m_pBlock->Release(); m_pBlock = 0; } }
};
struct CBase { ~CBase() { g_order[g_n++] = 1; } };
struct CMember { ~CMember() { g_order[g_n++] = 3; } };
class CGraphicInfoFrame { public:
    CBase          m_base;
    CSharedPointer m_pShared;
    long           m_frame;
};
int main()
{
    int fails = 0;
    SSharedBlock block; block.m_references = 2;
    CGraphicInfoFrame* p = new CGraphicInfoFrame; p->m_pShared.m_pBlock = &block;
    delete p;
    if (block.m_references != 1 || g_destroyed != 0) ++fails;
    if (g_n != 2 || g_order[0] != 2 || g_order[1] != 1) ++fails;
    g_n = 0;
    p = new CGraphicInfoFrame; p->m_pShared.m_pBlock = &block;
    delete p;
    if (block.m_references != 0 || g_destroyed != 1) ++fails;
    g_n = 0;
    p = new CGraphicInfoFrame; p->m_pShared.m_pBlock = 0;
    delete p;
    if (g_destroyed != 1 || g_n != 2) ++fails;
    if (fails) { std::printf("00a009a0_TEST FAIL\n"); return 1; }
    std::printf("00a009a0_TEST PASS\n");
    return 0;
}