// behaviour test for 007dff90 CTCTeleporter::StartTeleporting: clears the map flags; with no GUI block, world mode 0, a
// built teleport UI and a live hero with health, queues event 0x1a (payload 8) for the main player; otherwise sets
// InventoryScreen back on
#include <cstdio>
#include <cstring>
struct CTCBase;
struct CTCInventoryMap {
    bool InventoryScreen;
    bool OpenedViaGuildSeal;
    bool m_build;
    int  m_calls;
    void BuildMapTeleportUI(long screen, bool forceRebuild, bool* built) { ++m_calls; if (screen == 0 && forceRebuild) *built = m_build; }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
class CThing {
public:
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char m_flags91;
    float         Health;
    template<class T> bool GetInterface(T** ppOut, long type)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            *ppOut = (T*)((it == map->last || type < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CPlayerGui { bool m_blockingFD; };
static CPlayerGui g_gui; CPlayerGui* g_pPlayerGui = &g_gui;
float g_epsilon_0129ba3c = 1e-4f;
static unsigned long g_payload = 0; static int g_events = 0; static long g_eventType = 0; static int g_eventPlayer = 0;
struct CMemoryBuffer { void Copy(unsigned long length, const void* data) { if (length == 4) std::memcpy(&g_payload, data, 4); } };
struct CGameEvent : CMemoryBuffer {
    long          Type;
    signed char   Player;
    unsigned char Data[32];
    unsigned char EndPos;
    bool          Valid;
    bool          Replacement;
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddULONG(unsigned long value) { Copy(4, &value); }
};
struct CMainGameComponent { void AddGameEvent(const CGameEvent& e) { ++g_events; g_eventType = e.Type; g_eventPlayer = e.Player; } };
static CMainGameComponent g_main; CMainGameComponent* g_pMainGameComponent = &g_main;
struct CWorld { long m_mode; };
static CWorld g_world;
struct CPlayerManager { long GetMainPlayerNumber() { return 1; } };
static CPlayerManager g_players;
struct CTCBaseModel {
    virtual void v00() {}
    CThing* ParentThing;
    const CWorld* GetWorld() const { return &g_world; }
    CPlayerManager* GetPlayerManager() const { return &g_players; }
};
struct CTCTeleporter : CTCBaseModel {
    void StartTeleporting(CThing* hero);
};
void CTCTeleporter::StartTeleporting(CThing* hero)
{
    CTCInventoryMap* map;
    hero->GetInterface(&map, 0x1b);
    map->InventoryScreen = false;
    map->OpenedViaGuildSeal = false;
    if (!g_pPlayerGui->m_blockingFD && GetWorld()->m_mode == 0) {
        bool built = false;
        map->BuildMapTeleportUI(0, true, &built);
        if (built && !(hero->m_flags91 & 1) && hero->Health > g_epsilon_0129ba3c) {
            CGameEvent event(0x1a);
            event.AddULONG(8);
            event.Player = (signed char)GetPlayerManager()->GetMainPlayerNumber();
            g_pMainGameComponent->AddGameEvent(event);
            return;
        }
    }
    map->InventoryScreen = true;
}
int main()
{
    CTCInventoryMap invMap; invMap.m_build = true; invMap.m_calls = 0; invMap.InventoryScreen = true; invMap.OpenedViaGuildSeal = true;
    CInterfaceMap::SEntry entries[1] = { { 0x1b, (CTCBase*)&invMap } };
    CThing hero; std::memset(hero.m_presentBits, 0, sizeof(hero.m_presentBits)); hero.m_presentBits[0] = 1UL << 27;
    hero.m_interfaces.first = entries; hero.m_interfaces.last = entries + 1; hero.m_flags91 = 0; hero.Health = 10.0f;
    CTCTeleporter teleporter; g_gui.m_blockingFD = false; g_world.m_mode = 0;
    teleporter.StartTeleporting(&hero);
    bool ok = g_events == 1 && g_eventType == 0x1a && g_eventPlayer == 1 && g_payload == 8 && !invMap.InventoryScreen &&
              !invMap.OpenedViaGuildSeal && invMap.m_calls == 1;
    hero.Health = 0.0f; invMap.OpenedViaGuildSeal = true;
    teleporter.StartTeleporting(&hero);
    ok = ok && g_events == 1 && invMap.InventoryScreen && !invMap.OpenedViaGuildSeal;
    g_gui.m_blockingFD = true; hero.Health = 10.0f;
    teleporter.StartTeleporting(&hero);
    ok = ok && g_events == 1 && invMap.InventoryScreen && invMap.m_calls == 2;
    if (!ok) { std::printf("007dff90_TEST FAIL\n"); return 1; }
    std::printf("007dff90_TEST PASS\n");
    return 0;
}