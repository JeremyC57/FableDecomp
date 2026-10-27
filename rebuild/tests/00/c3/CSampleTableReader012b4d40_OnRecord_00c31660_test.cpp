// Behaviour test for 0x00C31660 CSampleTableReader_012b4d40::OnRecord
#include <cstdio>
#include <cstring>
struct Entry { const char* m_key; unsigned int m_values[8]; unsigned int m_count; };
static Entry g_entries[4]; static int g_nEntries = 0; static int g_creates = 0;
class CMap { public:
    Entry* Find(const char* key) { for (int i = 0; i < g_nEntries; ++i) if (!std::strcmp(g_entries[i].m_key, key)) return &g_entries[i]; return 0; }
    Entry* CreateEntry(const char* key) { ++g_creates; g_entries[g_nEntries].m_key = key; g_entries[g_nEntries].m_count = 0; return &g_entries[g_nEntries++]; }
    Entry* Get(const char* key) { if (Entry* e = Find(key)) return e; return CreateEntry(key); }
};
struct CReader {
    CMap m_samples;
    void OnRecord(const char* name, unsigned int count, const unsigned int* values)
    {
        Entry* entry = m_samples.Get(name);
        entry->m_count = count;
        for (unsigned int i = 0; i < count; ++i) entry->m_values[i] = values[i];
    }
};
int main()
{
    CReader r; unsigned int v[3] = { 4, 5, 6 };
    r.OnRecord("Hit", 3, v);
    r.OnRecord("Miss", 1, v + 2);
    unsigned int w[2] = { 7, 8 };
    r.OnRecord("Hit", 2, w);
    if (g_creates != 2 || g_entries[0].m_count != 2 || g_entries[0].m_values[1] != 8 || g_entries[1].m_values[0] != 6) {
        std::printf("00c31660_TEST FAIL\n"); return 1;
    }
    std::printf("00c31660_TEST PASS\n");
    return 0;
}