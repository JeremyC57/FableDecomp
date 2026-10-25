// Behaviour test for CResourceRegistry::SetEntryValue @ 0x00C2A300 (with its siblings HasEntry 0x00C2A320 and ReleaseOwnerEntries 0x00C2A640).
#include <cstdio>

struct CResourceEntry {
    unsigned long m_key;
    unsigned long m_owner;
    long          m_value;
    bool          m_live;
};

static CResourceEntry g_entries[4];

struct CRegistryMap {                       // a keyed list over the shared entry table (by key or by owner)
    bool m_byOwner;
    virtual ~CRegistryMap() { }
    virtual CResourceEntry* Find(const unsigned long& key)
    {
        for (int i = 0; i < 4; ++i)
            if (g_entries[i].m_live && (m_byOwner ? g_entries[i].m_owner : g_entries[i].m_key) == key)
                return &g_entries[i];
        return 0;
    }
};

struct CResourceRegistry {
    CRegistryMap m_entries;
    CRegistryMap m_byOwner;
    int released;
    void ReleaseEntry(CResourceEntry* entry) { entry->m_live = false; ++released; }
    void SetEntryValue(unsigned long key, long value)
    {
        CResourceEntry* entry = m_entries.Find(key);
        if (entry)
            entry->m_value = value;
    }
    bool HasEntry(unsigned long key) { return m_entries.Find(key) != 0; }
    void ReleaseOwnerEntries(void* owner)
    {
        CResourceEntry* entry;
        while ((entry = m_byOwner.Find((unsigned long)owner)) != 0)
            ReleaseEntry(entry);
    }
};

int main()
{
    int ownerA, ownerB;
    for (int i = 0; i < 4; ++i) {
        g_entries[i].m_key = 100 + i;
        g_entries[i].m_owner = (unsigned long)((i & 1) ? &ownerB : &ownerA);
        g_entries[i].m_value = 0;
        g_entries[i].m_live = true;
    }
    CResourceRegistry r;
    r.m_entries.m_byOwner = false; r.m_byOwner.m_byOwner = true; r.released = 0;
    bool ok = true;
    r.SetEntryValue(102, 77);
    r.SetEntryValue(999, 5);                                // unknown key: nothing written
    if (g_entries[2].m_value != 77 || g_entries[0].m_value != 0) ok = false;
    if (!r.HasEntry(101) || r.HasEntry(999)) ok = false;
    r.ReleaseOwnerEntries(&ownerB);
    if (r.released != 2 || g_entries[1].m_live || g_entries[3].m_live || !g_entries[0].m_live) ok = false;
    if (r.HasEntry(101) || !r.HasEntry(100)) ok = false;
    r.ReleaseOwnerEntries(&ownerB);                         // nothing left for that owner
    if (r.released != 2) ok = false;
    if (!ok) { std::printf("00c2a300_TEST FAIL\n"); return 1; }
    std::printf("00c2a300_TEST PASS\n");
    return 0;
}