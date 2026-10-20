// Behaviour test for CTCHeroStats::UpdateOutstandingCrimes (retail 0x0057D9FD): model copy + stub callees.
#include <cstdio>

class CTCBase;
struct CCharString { int id; };
struct CPresenceBits { unsigned long bits; bool test(long kind) { return ((bits >> (kind & 31)) & 1) != 0 && kind == 0x51; } };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};
class CThing {
public:
    CPresenceBits present;
    CInterfaceMap m_interfaces;
    bool HasTC(long type) { return present.test(type); }
    CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
};
static int g_destroyed = 0;
struct COpinionDeedEffectsDef {
    long m_count;
    long KnowledgeLifeInSeconds;
    void ReleaseTemporary() { if (--m_count == 0) ++g_destroyed; }
};
template <class T> struct CDefPointer {
    T* Object;
    explicit CDefPointer(T* object) : Object(object) { ++Object->m_count; }
    T* operator->() const { return Object; }
    ~CDefPointer() { if (Object) { Object->ReleaseTemporary(); Object = 0; } }
};
static COpinionDeedEffectsDef g_defs[10];
static int g_fetches = 0;
struct CTCHeroOpinionDeedLog {
    CDefPointer<COpinionDeedEffectsDef> GetDeedEffectsDef(long deedType)
    {
        ++g_fetches;
        return CDefPointer<COpinionDeedEffectsDef>(&g_defs[deedType]);   // a returned temporary: no extra copy/release (VC7.1 has no NRVO)
    }
};
struct MirroredCriminalStatus { long Crimes[10]; long LastTimeSeenByGuard; };
struct CCrimeStatusMap {
    MirroredCriminalStatus regions[2];
    MirroredCriminalStatus& operator[](const CCharString& key) { return regions[key.id]; }
};
static long g_frame = 1000;
struct CWorld { static long GetWorldFrame() { return g_frame; } };
struct CTCHeroStats {
    CThing*         ParentThing;
    CCrimeStatusMap OutstandingCrimes;
    CWorld* GetWorld() const { static CWorld w; return &w; }
    void UpdateOutstandingCrimes(const CCharString& region);
};
void CTCHeroStats::UpdateOutstandingCrimes(const CCharString& region)
{
    CTCHeroOpinionDeedLog* deedLog = 0;
    CThing* parent = ParentThing;
    if (parent->HasTC(0x51))
        deedLog = (CTCHeroOpinionDeedLog*)parent->GetTC(0x51);
    for (long crime = 0; crime < 10; ++crime) {
        if (OutstandingCrimes[region].Crimes[crime] != 0) {
            deedLog->GetDeedEffectsDef(crime);
            long expiry = deedLog->GetDeedEffectsDef(crime)->KnowledgeLifeInSeconds * 15
                        + OutstandingCrimes[region].LastTimeSeenByGuard;
            if (expiry < GetWorld()->GetWorldFrame())
                OutstandingCrimes[region].Crimes[crime] = 0;
        }
    }
}
int main()
{
    int fail = 0;
    for (int i = 0; i < 10; ++i) { g_defs[i].m_count = 1; g_defs[i].KnowledgeLifeInSeconds = 10 * i; }
    CTCHeroOpinionDeedLog log;
    CInterfaceMap::SEntry entries[1] = { { 0x51, (CTCBase*)&log } };
    CThing hero; hero.present.bits = 1UL << (0x51 & 31); hero.m_interfaces.first = entries; hero.m_interfaces.last = entries + 1;
    CTCHeroStats stats; stats.ParentThing = &hero;
    for (int r = 0; r < 2; ++r) { for (int i = 0; i < 10; ++i) stats.OutstandingCrimes.regions[r].Crimes[i] = 0; stats.OutstandingCrimes.regions[r].LastTimeSeenByGuard = 700; }
    stats.OutstandingCrimes.regions[1].Crimes[1] = 3;   // expiry 700 + 150 = 850 < 1000 -> cleared
    stats.OutstandingCrimes.regions[1].Crimes[4] = 2;   // expiry 700 + 600 = 1300 -> kept
    stats.OutstandingCrimes.regions[0].Crimes[1] = 5;   // other region untouched
    CCharString region = { 1 };
    stats.UpdateOutstandingCrimes(region);
    if (stats.OutstandingCrimes.regions[1].Crimes[1] != 0 || stats.OutstandingCrimes.regions[1].Crimes[4] != 2 || stats.OutstandingCrimes.regions[0].Crimes[1] != 5) fail = 1;
    if (g_fetches != 4 || g_defs[1].m_count != 1 || g_defs[4].m_count != 1 || g_destroyed != 0) fail = 2;
    if (fail) { std::printf("0057d9fd_TEST FAIL\n"); return 1; }
    std::printf("0057d9fd_TEST PASS\n");
    return 0;
}
