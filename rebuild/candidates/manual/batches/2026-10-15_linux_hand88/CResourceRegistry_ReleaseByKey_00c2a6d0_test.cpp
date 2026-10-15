#include <cstdio>

static void* g_released = 0; static int g_releases = 0; static unsigned long g_lookedUp = 0;
struct CRegistryMap {
    void* m_entry; unsigned long m_key;
    virtual ~CRegistryMap() { }
    virtual void* Find(const unsigned long* key) { g_lookedUp = *key; return *key == m_key ? m_entry : 0; }
};
struct CResourceRegistry {
    CRegistryMap m_entries;
    void ReleaseEntry(void* entry) { g_released = entry; ++g_releases; }
    void ReleaseByKey(unsigned long key)
    {
        void* entry = m_entries.Find(&key);
        if (entry)
            ReleaseEntry(entry);
    }
};

int main()
{
    int payload; CResourceRegistry r; r.m_entries.m_entry = &payload; r.m_entries.m_key = 42;
    r.ReleaseByKey(7);
    if (g_lookedUp != 7 || g_releases != 0) { std::printf("00c2a6d0_TEST FAIL\n"); return 1; }
    r.ReleaseByKey(42);
    if (g_releases != 1 || g_released != &payload) { std::printf("00c2a6d0_TEST FAIL\n"); return 1; }
    std::printf("00c2a6d0_TEST PASS\n");
    return 0;
}
