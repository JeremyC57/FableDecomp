// Behaviour test for 00c36290: CResourceWorld::FreeAllocation -- detach the owner's allocation from the registry and delete it (nothing when absent)
#include <cstdio>
static int g_destroyed = 0;
struct CResourceAllocation { void* owner; ~CResourceAllocation() { ++g_destroyed; } };
struct CAllocationRegistry {
    CResourceAllocation* slot; void* lastKey;
    virtual CResourceAllocation* Detach(void* const* key)
    {
        lastKey = *key;
        if (slot && slot->owner == *key) { CResourceAllocation* a = slot; slot = 0; return a; }
        return 0;
    }
};
struct CResourceWorld {
    CAllocationRegistry m_registry;
    CResourceAllocation* DetachAllocation(void* owner) { return m_registry.Detach(&owner); }
    void FreeAllocation(void* owner) { delete DetachAllocation(owner); }
};
int main()
{
    int ownerA = 0, ownerB = 0;
    CResourceWorld w; w.m_registry.slot = new CResourceAllocation; w.m_registry.slot->owner = &ownerA; w.m_registry.lastKey = 0;
    w.FreeAllocation(&ownerB);
    if (g_destroyed != 0 || w.m_registry.slot == 0 || w.m_registry.lastKey != &ownerB) { std::printf("00c36290_TEST FAIL\n"); return 1; }
    w.FreeAllocation(&ownerA);
    if (g_destroyed != 1 || w.m_registry.slot != 0) { std::printf("00c36290_TEST FAIL\n"); return 1; }
    std::printf("00c36290_TEST PASS\n");
    return 0;
}