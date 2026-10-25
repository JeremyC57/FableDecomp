// Behaviour test for 00c362f0: CResourceWorld::AllocateForAll -- a stack visitor bound to the world visits every owner and allocates for it
#include <cstdio>
struct CResourceWorld;
struct IItemVisitor {
    virtual void Visit(void* item) = 0;
    virtual ~IItemVisitor() { }
};
struct CAllocateVisitor : IItemVisitor {
    CResourceWorld* m_pWorld;
    CAllocateVisitor(CResourceWorld* world) : m_pWorld(world) { }
    virtual void Visit(void* item);
};
struct IItemCollection {
    void* m_items[3]; int m_count;
    void ForEach(IItemVisitor* visitor) { for (int i = 0; i < m_count; ++i) visitor->Visit(m_items[i]); }
};
struct COwnerSet { IItemCollection m_items; };
struct CResourceWorld {
    COwnerSet* m_pOwners; void* m_allocated[3]; int m_count;
    void AllocateFor(void* owner) { m_allocated[m_count++] = owner; }
    void AllocateForAll()
    {
        CAllocateVisitor visitor(this);
        m_pOwners->m_items.ForEach(&visitor);
    }
};
void CAllocateVisitor::Visit(void* item) { m_pWorld->AllocateFor(item); }
int main()
{
    static int a, b, c;
    COwnerSet owners; owners.m_items.m_items[0] = &a; owners.m_items.m_items[1] = &b; owners.m_items.m_items[2] = &c;
    owners.m_items.m_count = 3;
    CResourceWorld world; world.m_pOwners = &owners; world.m_count = 0;
    world.AllocateForAll();
    int ok = world.m_count == 3 && world.m_allocated[0] == &a && world.m_allocated[1] == &b && world.m_allocated[2] == &c;
    owners.m_items.m_count = 0; world.m_count = 0;
    world.AllocateForAll();
    ok = ok && world.m_count == 0;
    std::printf(ok ? "00c362f0_TEST PASS\n" : "00c362f0_TEST FAIL\n");
    return ok ? 0 : 1;
}