#include <cstdio>

// NParticleEngine::CPSCManager::CPSCManager(): empty map, RegisterPSCs, then one sort (with the map's
// comparator) only if the registrations left the map dirty, and Dirty cleared.
static int g_sorts = 0;
static int g_registerMode = 0;
namespace NParticleEngine {
struct SRegisteredPSCEntry { unsigned char _body[0x10]; };
struct CKeyPairCompareLess { };
static SRegisteredPSCEntry g_entries[3];
static SRegisteredPSCEntry* g_sortFirst = 0;
static SRegisteredPSCEntry* g_sortLast = 0;
void SortRegisteredPSCs(SRegisteredPSCEntry* first, SRegisteredPSCEntry* last, CKeyPairCompareLess)
{
    ++g_sorts; g_sortFirst = first; g_sortLast = last;
}
struct CRegisteredPSCMap {
    SRegisteredPSCEntry* m_first;
    SRegisteredPSCEntry* m_last;
    SRegisteredPSCEntry* m_end;
    CKeyPairCompareLess  Compare;
    bool                 Dirty;
    CRegisteredPSCMap() : m_first(0), m_last(0), m_end(0), Dirty(false) { }
    void SortIfDirty()
    {
        if (Dirty) {
            SortRegisteredPSCs(m_first, m_last, Compare);
            Dirty = false;
        }
    }
};
class CPSCManager { public:
    CRegisteredPSCMap RegisteredPSCs;
    void RegisterPSCs()
    {
        if (g_registerMode) {
            RegisteredPSCs.m_first = g_entries; RegisteredPSCs.m_last = g_entries + 3; RegisteredPSCs.m_end = g_entries + 3;
            RegisteredPSCs.Dirty = true;
        }
    }
    CPSCManager();
};
CPSCManager::CPSCManager()
{
    RegisterPSCs();
    RegisteredPSCs.SortIfDirty();
}
}

int main()
{
    using namespace NParticleEngine;
    CPSCManager clean;
    bool ok = g_sorts == 0 && clean.RegisteredPSCs.m_first == 0 && clean.RegisteredPSCs.m_last == 0 && !clean.RegisteredPSCs.Dirty;
    g_registerMode = 1;
    CPSCManager dirty;
    ok = ok && g_sorts == 1 && g_sortFirst == g_entries && g_sortLast == g_entries + 3 && !dirty.RegisteredPSCs.Dirty;
    if (!ok) {
        std::printf("00af2970_TEST FAIL\n");
        return 1;
    }
    std::printf("00af2970_TEST PASS\n");
    return 0;
}