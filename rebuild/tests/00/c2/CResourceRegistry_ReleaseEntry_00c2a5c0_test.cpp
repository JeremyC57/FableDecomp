#include <cstdio>
#include <new>
#include <cstdlib>
static int g_entryDtor = 0, g_mapDtor = 0, g_frees = 0; static long g_inUse = 1;
void operator delete(void* p) throw() { ++g_frees; std::free(p); }
struct CEntryList { virtual long IsInUse() { return g_inUse; } };
struct CResourceMap { unsigned char pad[0x0c]; CEntryList m_entries; ~CResourceMap() { ++g_mapDtor; } };
struct CResourceEntry { CResourceMap* m_pOwner; ~CResourceEntry() { ++g_entryDtor; } };
class CResourceRegistry {
public:
    void ReleaseEntry(CResourceEntry* entry)
    {
        CResourceMap* owner = entry->m_pOwner;
        entry->~CResourceEntry();
        operator delete(entry);
        if (owner->m_entries.IsInUse() == 0) { owner->~CResourceMap(); operator delete(owner); }
    }
};
int main()
{
    CResourceRegistry r;
    CResourceMap* map = new (std::malloc(sizeof(CResourceMap))) CResourceMap;
    CResourceEntry* a = new (std::malloc(sizeof(CResourceEntry))) CResourceEntry; a->m_pOwner = map;
    CResourceEntry* b = new (std::malloc(sizeof(CResourceEntry))) CResourceEntry; b->m_pOwner = map;
    r.ReleaseEntry(a);
    bool ok = g_entryDtor == 1 && g_mapDtor == 0 && g_frees == 1;
    g_inUse = 0;
    r.ReleaseEntry(b);
    ok = ok && g_entryDtor == 2 && g_mapDtor == 1 && g_frees == 3;
    if (!ok) { std::printf("00c2a5c0_TEST FAIL\n"); return 1; }
    std::printf("00c2a5c0_TEST PASS\n");
    return 0;
}
