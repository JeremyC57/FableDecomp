#include <cstdio>
// Behaviour test for CCreatureAction_PerformExpressionExtended scalar deleting destructor (0x0084B620): shared block released on last ref, members destroyed in reverse order, base last.
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
class CCreatureAction_PerformExpressionExtended { public:
    CBase          m_base;
    CSharedPointer m_pShared;
    CMember        m_last;       // the member destroyed first (intelligent pointer / declared after)
};
int main()
{
    int fails = 0;
    SSharedBlock block; block.m_references = 2;
    CCreatureAction_PerformExpressionExtended* p = new CCreatureAction_PerformExpressionExtended; p->m_pShared.m_pBlock = &block;
    delete p;
    if (block.m_references != 1 || g_destroyed != 0) ++fails;
    if (g_n != 3 || g_order[0] != 3 || g_order[1] != 2 || g_order[2] != 1) ++fails;
    g_n = 0;
    p = new CCreatureAction_PerformExpressionExtended; p->m_pShared.m_pBlock = &block;
    delete p;
    if (block.m_references != 0 || g_destroyed != 1) ++fails;
    g_n = 0;
    p = new CCreatureAction_PerformExpressionExtended; p->m_pShared.m_pBlock = 0;
    delete p;
    if (g_destroyed != 1 || g_n != 3) ++fails;
    if (fails) { std::printf("0084b620_TEST FAIL\n"); return 1; }
    std::printf("0084b620_TEST PASS\n");
    return 0;
}