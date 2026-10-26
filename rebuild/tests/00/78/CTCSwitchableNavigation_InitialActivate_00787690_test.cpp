// Behaviour test for CTCSwitchableNavigation::InitialActivate (00787690): the thing's 40-bit id is (un)blocked in its map only when
// that map is activated.
#include <cstdio>

#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif

static int g_setCalls = 0;
static long g_setMap = 0;
static u64 g_setUid = 0;
static bool g_setBlocked = false;

class CNavigatorManager { public:
    long m_activeMap;
    bool IsNavigationMapActivated(long mapIndex) const { return mapIndex == m_activeMap; }
    void SetSwitchableNodesAsBlocked(long mapIndex, u64 uid, bool blocked) { ++g_setCalls; g_setMap = mapIndex; g_setUid = uid; g_setBlocked = blocked; }
};

class CWorld { public: CNavigatorManager* PNavigationManager; };
static CNavigatorManager g_nav;
static CWorld g_world = { &g_nav };

class CThing { public: u64 UniqueID; };

static const u64 kUid40 = (((u64)0xFF) << 32) | 0xFFFFFFFFUL;

class CTCSwitchableNavigation { public:
    CThing* ParentThing;
    long    MapIndex;
    CWorld* GetWorld() { return &g_world; }
    void InitialActivate();
};

void CTCSwitchableNavigation::InitialActivate()
{
    CNavigatorManager* navigation = GetWorld()->PNavigationManager;
    if (navigation->IsNavigationMapActivated(MapIndex))
        navigation->SetSwitchableNodesAsBlocked(MapIndex, ParentThing->UniqueID & kUid40, true);
}

int main()
{
    CThing thing; thing.UniqueID = (((u64)0x12345) << 32) | 0x6789ABCDUL;
    CTCSwitchableNavigation n; n.ParentThing = &thing; n.MapIndex = 4;
    g_nav.m_activeMap = 5;
    n.InitialActivate();                                                    // map not activated
    if (g_setCalls) { std::printf("00787690_TEST FAIL\n"); return 1; }
    g_nav.m_activeMap = 4;
    n.InitialActivate();
    if (g_setCalls != 1 || g_setMap != 4 || g_setUid != ((((u64)0x45) << 32) | 0x6789ABCDUL) || g_setBlocked != true) { std::printf("00787690_TEST FAIL\n"); return 1; }
    n.InitialActivate();
    if (g_setCalls != 2 || g_setBlocked != true) { std::printf("00787690_TEST FAIL\n"); return 1; }
    std::printf("00787690_TEST PASS\n");
    return 0;
}