// Behaviour test for 00c36180: CResourceWorld::AllocateFor -- the new allocation registers itself with the world
#include <cstdio>
struct CResourceWorld;
struct CResourceAllocation {
    CResourceWorld* m_world; void* m_owner; unsigned char _rest[0x84];
    CResourceAllocation(CResourceWorld* world, void* owner);
};
struct CResourceWorld {
    CResourceAllocation* m_registered; int m_count;
    void AllocateFor(void* owner) { new CResourceAllocation(this, owner); }
};
CResourceAllocation::CResourceAllocation(CResourceWorld* world, void* owner) : m_world(world), m_owner(owner)
{
    world->m_registered = this; ++world->m_count;
}
int main()
{
    if (sizeof(CResourceAllocation) < 0x8c) { std::printf("00c36180_TEST FAIL\n"); return 1; }
    CResourceWorld w; w.m_registered = 0; w.m_count = 0;
    int owner = 0;
    w.AllocateFor(&owner);
    if (w.m_count != 1 || w.m_registered == 0 || w.m_registered->m_owner != &owner || w.m_registered->m_world != &w) { std::printf("00c36180_TEST FAIL\n"); return 1; }
    delete w.m_registered;
    std::printf("00c36180_TEST PASS\n");
    return 0;
}