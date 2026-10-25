// Behaviour test for 00c36270: CAllocateVisitor constructor -- keeps the world, dispatches Visit through the interface
#include <cstdio>
struct CResourceWorld { int m_allocs; void AllocateFor(void*) { ++m_allocs; } };
struct IItemVisitor { virtual void Visit(void* item) = 0; virtual ~IItemVisitor() {} };
struct CAllocateVisitor : IItemVisitor {
    CResourceWorld* m_pWorld;
    CAllocateVisitor(CResourceWorld* world) : m_pWorld(world) {}
    virtual void Visit(void* item) { m_pWorld->AllocateFor(item); }
};
int main()
{
    CResourceWorld w; w.m_allocs = 0; CAllocateVisitor v(&w); IItemVisitor* p = &v; int a, b; p->Visit(&a); p->Visit(&b);
    if (v.m_pWorld != &w || w.m_allocs != 2 || sizeof(v) != 8) { std::printf("00c36270_TEST FAIL\n"); return 1; }
    std::printf("00c36270_TEST PASS\n");
    return 0;
}
