// Behaviour test for CTCHeroStats::GetOutstandingFineAndFramesLeft (retail 0x0057DAF5): model copy + stub callees.
#include <cstdio>

class CTCBase;
class CCharString { public: int id; };
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
class COpinionDeedEffectsDef { public:
    long m_count;
    long KnowledgeLifeInSeconds;
    long CrimeFineAmount;
    void ReleaseTemporary() { if (--m_count == 0) ++g_destroyed; }
};
template <class T> class CDefPointer { public:
    T* Object;
    explicit CDefPointer(T* object) : Object(object) { ++Object->m_count; }
    T* operator->() const { return Object; }
    ~CDefPointer() { if (Object) { Object->ReleaseTemporary(); Object = 0; } }
};
static COpinionDeedEffectsDef g_defs[10];
static int g_fetches = 0;
class CTCHeroOpinionDeedLog { public:
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
class CWorld { public: static long __fastcall GetFrame() { return g_frame; } };
class CTCHeroStats { public:
    CThing*         ParentThing;
    CCrimeStatusMap OutstandingCrimes;
    CWorld* GetWorld() const { static CWorld w; return &w; }
    void UpdateOutstandingCrimes(const CCharString& region);
    void GetOutstandingFineAndFramesLeft(const CCharString& region, long& fine, long& framesLeft);
};
template <class T> inline const T& Max(const T& left, const T& right) { return left < right ? right : left; }
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
            if (expiry < GetWorld()->GetFrame())
                OutstandingCrimes[region].Crimes[crime] = 0;
        }
    }
}
void CTCHeroStats::GetOutstandingFineAndFramesLeft(const CCharString& region, long& fine, long& framesLeft)
{
    UpdateOutstandingCrimes(region);
    framesLeft = 0;
    fine = 0;
    CTCHeroOpinionDeedLog* deedLog = 0;
    CThing* parent = ParentThing;
    if (parent->HasTC(0x51))
        deedLog = (CTCHeroOpinionDeedLog*)parent->GetTC(0x51);
    for (long crime = 0; crime < 10; ++crime) {
        if (OutstandingCrimes[region].Crimes[crime] != 0) {
            fine += deedLog->GetDeedEffectsDef(crime)->CrimeFineAmount * OutstandingCrimes[region].Crimes[crime];
            long expiry = deedLog->GetDeedEffectsDef(crime)->KnowledgeLifeInSeconds * 15
                        + OutstandingCrimes[region].LastTimeSeenByGuard;
            framesLeft = Max(framesLeft, expiry - GetWorld()->GetFrame());
        }
    }
}
int main()
{
    int fail = 0;
    for (int i = 0; i < 10; ++i) { g_defs[i].m_count = 1; g_defs[i].KnowledgeLifeInSeconds = 10 * i; g_defs[i].CrimeFineAmount = 5 * (i + 1); }
    CTCHeroOpinionDeedLog log;
    CInterfaceMap::SEntry entries[1] = { { 0x51, (CTCBase*)&log } };
    CThing hero; hero.present.bits = 1UL << (0x51 & 31); hero.m_interfaces.first = entries; hero.m_interfaces.last = entries + 1;
    CTCHeroStats stats; stats.ParentThing = &hero;
    for (int r = 0; r < 2; ++r) { for (int i = 0; i < 10; ++i) stats.OutstandingCrimes.regions[r].Crimes[i] = 0; stats.OutstandingCrimes.regions[r].LastTimeSeenByGuard = 700; }
    stats.OutstandingCrimes.regions[1].Crimes[1] = 3;   // expires (850 < 1000): dropped first
    stats.OutstandingCrimes.regions[1].Crimes[4] = 2;   // fine 2 * 25 = 50, frames left 1300 - 1000 = 300
    stats.OutstandingCrimes.regions[1].Crimes[6] = 1;   // fine 1 * 35 = 35, frames left 1600 - 1000 = 600
    CCharString region = { 1 };
    long fine = -1, framesLeft = -1;
    stats.GetOutstandingFineAndFramesLeft(region, fine, framesLeft);
    if (fine != 85 || framesLeft != 600 || stats.OutstandingCrimes.regions[1].Crimes[1] != 0) fail = 1;
    if (g_defs[4].m_count != 1 || g_defs[6].m_count != 1 || g_destroyed != 0) fail = 2;
    CCharString clean = { 0 };
    stats.GetOutstandingFineAndFramesLeft(clean, fine, framesLeft);
    if (fine != 0 || framesLeft != 0) fail = 3;
    if (fail) { std::printf("0057daf5_TEST FAIL\n"); return 1; }
    std::printf("0057daf5_TEST PASS\n");
    return 0;
}