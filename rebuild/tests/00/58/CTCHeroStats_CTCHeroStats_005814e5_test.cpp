// Behaviour test for 0x005814E5 CTCHeroStats::~CTCHeroStats (copy of the member model over spy callees): reverse member
// destruction order, the primitive handle's last-handle notification, the def release at zero, the vector frees.
#include <cstdio>
#include <cstring>
static char g_order[64]; static int g_n = 0;
static void mark(char c) { if (g_n < 63) g_order[g_n++] = c; g_order[g_n] = 0; }
static int g_frees = 0;
static void spy_free(void* p) { if (p) ++g_frees; }
struct CBaseClass { virtual ~CBaseClass() { mark('B'); } };
struct CEnginePrimitiveHandle;
struct CEnginePersistentPrimitiveBase {
    long m_refCount;
    int notified, destroyed;
    CEnginePersistentPrimitiveBase() : m_refCount(1), notified(0), destroyed(0) {}
    virtual ~CEnginePersistentPrimitiveBase() {}
    virtual void Destroy() { ++destroyed; }
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle*) { ++notified; }
};
template <class T> struct CIVCountedPointer {
    T* m_p;
    CIVCountedPointer() : m_p(0) { }
    ~CIVCountedPointer() { Release(); }
    void Release() { T* p = m_p; if (p) { if (--p->m_refCount == 0) p->Destroy(); m_p = 0; } }
    CIVCountedPointer& operator=(T* p) { Release(); m_p = p; return *this; }
    T* operator->() const { return m_p; }
    long GetRefCount() const { return m_p ? m_p->m_refCount : 0; }
};
struct CEnginePrimitiveHandle : CBaseClass {
    CIVCountedPointer<CEnginePersistentPrimitiveBase> Data;
    ~CEnginePrimitiveHandle() { mark('H'); if (Data.GetRefCount() == 1) Data->OnLastHandleReleased(this); Data = 0; }
};
struct CDefPointee {
    long m_refCount; int destroyed;
    CDefPointee() : m_refCount(1), destroyed(0) {}
    virtual ~CDefPointee() {}
    virtual void Destroy() { ++destroyed; }
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { mark('D'); T* p = Object; if (p) { if (--p->m_refCount == 0) p->Destroy(); Object = 0; } }
};
struct CCharString { ~CCharString() { mark('S'); } };
template <class T> struct CVectorStorage {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    CVectorStorage() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CVectorStorage() { mark('V'); if (_M_start) spy_free(_M_start); }
};
struct CCriminalStatusMap { ~CCriminalStatusMap() { mark('M'); } };
struct CMarriageList { ~CMarriageList() { mark('L'); } };
class CTCBase {
public:
    virtual ~CTCBase() { mark('T'); }
};
class CTCHeroStats : public CTCBase {
public:
    CCriminalStatusMap   OutstandingCrimes;
    CVectorStorage<long> ExperienceSpentOn;
    CVectorStorage<long> HeroStatsExperience;
    CVectorStorage<long> RegionTime;
    CVectorStorage<long> NumberOfTimesUsedAbility;
    CVectorStorage<long> HousePositions;
    CCharString          DeathRecoveryMarkerName;
    CDefPointer<CDefPointee> PDef;
    CEnginePrimitiveHandle   PrimitiveHandle;
    CMarriageList        Marriages;
    virtual ~CTCHeroStats();
};
CTCHeroStats::~CTCHeroStats()
{
}
int main()
{
    static long storage[4];
    CDefPointee def; CEnginePersistentPrimitiveBase prim;
    {
        CTCHeroStats* stats = new CTCHeroStats;
        stats->PDef.Object = &def;                       // last reference: destroyed
        stats->PrimitiveHandle.Data.m_p = &prim;         // last handle: notified, then destroyed
        stats->RegionTime._M_start = storage; stats->HousePositions._M_start = storage + 1;
        delete stats;
    }
    // members in reverse declaration order, then the CTCBase base
    if (std::strcmp(g_order, "LHBDSVVVVVMT") != 0) { std::printf("005814e5_TEST FAIL\n"); return 1; }
    if (def.destroyed != 1 || prim.notified != 1 || prim.destroyed != 1 || g_frees != 2) { std::printf("005814e5_TEST FAIL\n"); return 1; }
    std::printf("005814e5_TEST PASS\n");
    return 0;
}