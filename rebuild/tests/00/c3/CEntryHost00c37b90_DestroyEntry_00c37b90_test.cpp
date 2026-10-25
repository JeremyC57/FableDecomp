// Behaviour test for 00c37b90: CEntryHost::DestroyEntry -- looks the key up in the registry and deletes the entry (a null result is a no-op)
#include <cstdio>

static int g_destroyed = 0;
struct CRegisteredEntry {
    void* m_key;
    ~CRegisteredEntry() { ++g_destroyed; }
};

struct CEntryRegistry {
    CRegisteredEntry* m_entry;
    void* m_lastKey;
    CRegisteredEntry* Lookup(void* const* key)
    {
        m_lastKey = *key;
        return (m_entry && m_entry->m_key == *key) ? m_entry : 0;
    }
    CRegisteredEntry* Find(void* key) { return Lookup(&key); }
};

struct CEntryHost {
    CEntryRegistry m_registry;
    void DestroyEntry(void* key)
    {
        CRegisteredEntry* entry = m_registry.Find(key);
        delete entry;
    }
};

static int fail() { std::printf("00c37b90_TEST FAIL\n"); return 1; }

int main()
{
    int keyA, keyB;
    CEntryHost host;
    host.m_registry.m_entry = new CRegisteredEntry;
    host.m_registry.m_entry->m_key = &keyA;
    host.DestroyEntry(&keyB);                               // not found
    if (g_destroyed != 0 || host.m_registry.m_lastKey != &keyB) return fail();
    host.DestroyEntry(&keyA);
    if (g_destroyed != 1) return fail();
    std::printf("00c37b90_TEST PASS\n");
    return 0;
}