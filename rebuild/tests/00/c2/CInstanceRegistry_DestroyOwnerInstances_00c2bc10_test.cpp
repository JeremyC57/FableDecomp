// Behaviour test for CInstanceRegistry::DestroyOwnerInstances @ 0x00C2BC10 (destroy the owner's instances one Find at a time until none is left).
#include <cstdio>

struct CInstance { unsigned long m_owner; bool m_live; };
static CInstance g_instances[5];

struct CInstanceList {
    CInstance* Find(const unsigned long& key)
    {
        for (int i = 0; i < 5; ++i)
            if (g_instances[i].m_live && g_instances[i].m_owner == key)
                return &g_instances[i];
        return 0;
    }
};

struct CInstanceRegistry {
    CInstanceList m_byOwner;
    int destroyed;
    void DestroyInstance(CInstance* instance) { instance->m_live = false; ++destroyed; }
    void DestroyOwnerInstances(void* owner)
    {
        CInstance* instance;
        while ((instance = m_byOwner.Find((unsigned long)owner)) != 0)
            DestroyInstance(instance);
    }
};

int main()
{
    int a, b, c;
    unsigned long owners[5] = { (unsigned long)&a, (unsigned long)&b, (unsigned long)&a, (unsigned long)&a, (unsigned long)&b };
    for (int i = 0; i < 5; ++i) { g_instances[i].m_owner = owners[i]; g_instances[i].m_live = true; }
    CInstanceRegistry r;
    r.destroyed = 0;
    r.DestroyOwnerInstances(&a);
    bool ok = r.destroyed == 3 && !g_instances[0].m_live && g_instances[1].m_live && !g_instances[3].m_live;
    r.DestroyOwnerInstances(&c);
    ok = ok && r.destroyed == 3;
    r.DestroyOwnerInstances(&b);
    ok = ok && r.destroyed == 5 && !g_instances[4].m_live;
    if (!ok) { std::printf("00c2bc10_TEST FAIL\n"); return 1; }
    std::printf("00c2bc10_TEST PASS\n");
    return 0;
}