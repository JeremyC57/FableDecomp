// Behaviour test for 00c37990: CEntryHost::CreateEntry -- allocates a registered entry for the key; the entry's constructor registers it with the host
#include <cstdio>

struct CEntryHost;
struct CRegisteredEntry {
    CEntryHost* m_pHost;
    void* m_key;
    CRegisteredEntry(CEntryHost* host, void* key);
};

struct CEntryHost {
    CRegisteredEntry* m_entries[4];
    int m_count;
    void CreateEntry(void* key) { new CRegisteredEntry(this, key); }
};

CRegisteredEntry::CRegisteredEntry(CEntryHost* host, void* key) : m_pHost(host), m_key(key)
{
    host->m_entries[host->m_count++] = this;
}

int main()
{
    int a, b;
    CEntryHost host; host.m_count = 0;
    host.CreateEntry(&a);
    host.CreateEntry(&b);
    if (host.m_count != 2 || host.m_entries[0]->m_key != &a || host.m_entries[1]->m_key != &b
        || host.m_entries[1]->m_pHost != &host) { std::printf("00c37990_TEST FAIL\n"); return 1; }
    delete host.m_entries[0]; delete host.m_entries[1];
    std::printf("00c37990_TEST PASS\n");
    return 0;
}