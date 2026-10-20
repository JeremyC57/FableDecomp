// Behaviour test for CTCDRegionExit::SetUpPEntranceConnectedToFromUID (0x00779340).
#include <cstdio>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif

struct CThing { unsigned char Flags91; bool IsDead() const { return (Flags91 & 1) != 0; } };
struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) {}
    virtual ~CBaseIntelligentPointer() {}
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CThing* item) { m_pItem = item; }
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { T* item = other.GetPItem(); SetPItem(item); return *this; }
};
inline bool IsThingAlive(const CBaseIntelligentPointer& p) { return p.GetPItem() && !p.GetPItem()->IsDead(); }

static u64 g_askedUID = 0;
static CThing* g_found = 0;
struct CThingManager { CThing* GetThingByUID(u64 uid) { g_askedUID = uid; return g_found; } };
static bool g_allow = true;
struct CGameSubsystem { virtual bool IsSuppressed() { return g_allow; } };
static unsigned long g_askedMapUID = 0;
static long g_mapNumber = 0;
static bool g_used = false, g_loaded = false;
struct CWorldMap {
    virtual bool IsMapLoaded(long) const { return g_loaded; }
    virtual bool IsMapUsed(long) const { return g_used; }
    long GetMapNumberFromMapUID(unsigned long uid) const { g_askedMapUID = uid; return g_mapNumber; }
};
static CThingManager g_tm; static CGameSubsystem g_sub; static CWorldMap g_wm;
class CTCDRegionExit {
public:
    u64 EntranceConnectedToUID;
    CIntelligentPointer<CThing> PEntranceConnectedTo;
    CTCDRegionExit() : PEntranceConnectedTo(0) {}
    CThingManager*  GetThingManager() { return &g_tm; }
    CGameSubsystem* GetGameSubsystem() { return &g_sub; }
    CWorldMap*      GetWorldMap() { return &g_wm; }
    void SetUpPEntranceConnectedToFromUID()
    {
        PEntranceConnectedTo = CIntelligentPointer<CThing>(GetThingManager()->GetThingByUID(EntranceConnectedToUID));
        if (IsThingAlive(PEntranceConnectedTo))
            return;
        if (!GetGameSubsystem()->IsSuppressed())
            return;
        u64 mapUID = EntranceConnectedToUID >> 40;
        long mapNumber = GetWorldMap()->GetMapNumberFromMapUID((unsigned long)mapUID);
        if (mapNumber == 0)
            EntranceConnectedToUID = 0;
        else if (!GetWorldMap()->IsMapUsed(mapNumber) || GetWorldMap()->IsMapLoaded(mapNumber))
            EntranceConnectedToUID = 0;
    }
};

static u64 Run(CThing* found, bool allow, long mapNumber, bool used, bool loaded, CThing** resolved)
{
    const u64 uid = ((u64)0xABCDEF << 40) | 0x12345;
    g_found = found; g_allow = allow; g_mapNumber = mapNumber; g_used = used; g_loaded = loaded;
    CTCDRegionExit e; e.EntranceConnectedToUID = uid;
    e.SetUpPEntranceConnectedToFromUID();
    *resolved = e.PEntranceConnectedTo.GetPItem();
    if (g_askedUID != uid) return 1;
    return e.EntranceConnectedToUID;
}

int main()
{
    const u64 uid = ((u64)0xABCDEF << 40) | 0x12345;
    CThing alive = { 0 }, dead = { 1 }; CThing* r = 0;
    bool ok = true;
    ok = ok && Run(&alive, true, 0, false, false, &r) == uid && r == &alive;      // alive: kept
    ok = ok && Run(&dead, false, 0, false, false, &r) == uid && r == &dead;       // not allowed: kept
    g_askedMapUID = 0;
    ok = ok && Run(&dead, true, 0, true, false, &r) == 0 && g_askedMapUID == 0xABCDEF;   // unknown map
    ok = ok && Run(0, true, 3, false, false, &r) == 0 && r == 0;                  // map unused
    ok = ok && Run(0, true, 3, true, true, &r) == 0;                              // map loaded
    ok = ok && Run(0, true, 3, true, false, &r) == uid;                           // used, not loaded: kept
    if (!ok) { std::printf("00779340_TEST FAIL\n"); return 1; }
    std::printf("00779340_TEST PASS\n");
    return 0;
}
