#include <cstdio>
// Behaviour test for C3DMeshMaterial scalar deleting destructor (0x00ABFAA0): shared block released on last ref, members destroyed in reverse order, base last.
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
struct C3DMeshMaterial {
    CBase          m_base;
    CMember        m_name;       // +0x08 string: destroyed after the shared pointer
    CSharedPointer m_pShared;    // +0x2c
};
int main()
{
    int fails = 0;
    SSharedBlock block; block.m_references = 2;
    C3DMeshMaterial* p = new C3DMeshMaterial; p->m_pShared.m_pBlock = &block;
    delete p;
    if (block.m_references != 1 || g_destroyed != 0) ++fails;
    if (g_n != 3 || g_order[0] != 2 || g_order[1] != 3 || g_order[2] != 1) ++fails;
    g_n = 0;
    p = new C3DMeshMaterial; p->m_pShared.m_pBlock = &block;
    delete p;
    if (block.m_references != 0 || g_destroyed != 1) ++fails;
    g_n = 0;
    p = new C3DMeshMaterial; p->m_pShared.m_pBlock = 0;
    delete p;
    if (g_destroyed != 1 || g_n != 3) ++fails;
    if (fails) { std::printf("00abfaa0_TEST FAIL\n"); return 1; }
    std::printf("00abfaa0_TEST PASS\n");
    return 0;
}