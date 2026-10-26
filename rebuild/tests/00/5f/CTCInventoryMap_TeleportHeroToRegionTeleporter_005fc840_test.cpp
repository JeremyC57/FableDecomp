#include <cstdio>
// Behaviour of CTCInventoryMap::TeleportHeroToRegionTeleporter (0x005FC840): teleports only when hero and teleporter are alive.
class CRegion { public: long m_index; };
class CThing { public: unsigned char m_flags; };
static CThing g_hero, g_teleporter;
static CRegion g_regions[4];
static bool g_noTeleporter = false;
class CPlayer { public: CThing* GetCharacterThing_00487dd0() { return &g_hero; } };
static CPlayer g_player;
class CPlayerManager { public: CPlayer* GetMainPlayer() const { return &g_player; } };
static CPlayerManager g_players;
class CWorldMap { public: CRegion& GetRegion(long index) { return g_regions[index]; } };
static CWorldMap g_world;
class CTCInventoryMap { public:
    long m_teleports; CThing* m_lastTarget; long m_askedRegion;
    CPlayerManager* GetPlayerManager() { return &g_players; }
    CWorldMap* GetWorldMap() { return &g_world; }
    CThing* GetRegionTeleporter(CRegion* region) { m_askedRegion = region->m_index; return g_noTeleporter ? 0 : &g_teleporter; }
    void TeleportHeroToTeleporter(CThing* t) { ++m_teleports; m_lastTarget = t; }
    void TeleportHeroToRegionTeleporter(long regionIndex)
    {
        CThing* hero = GetPlayerManager()->GetMainPlayer()->GetCharacterThing_00487dd0();
        if (hero && !(hero->m_flags & 1)) {
            CThing* teleporter = GetRegionTeleporter(&GetWorldMap()->GetRegion(regionIndex));
            if (teleporter && !(teleporter->m_flags & 1))
                TeleportHeroToTeleporter(teleporter);
        }
    }
};
int main()
{
    for (long i = 0; i < 4; ++i) g_regions[i].m_index = i;
    CTCInventoryMap m; m.m_teleports = 0; m.m_lastTarget = 0; m.m_askedRegion = -1;
    g_hero.m_flags = 0; g_teleporter.m_flags = 0;
    m.TeleportHeroToRegionTeleporter(2);
    int bad = (m.m_teleports != 1 || m.m_lastTarget != &g_teleporter || m.m_askedRegion != 2);
    g_teleporter.m_flags = 1; m.TeleportHeroToRegionTeleporter(3);
    bad |= (m.m_teleports != 1 || m.m_askedRegion != 3);
    g_teleporter.m_flags = 0; g_hero.m_flags = 1; m.m_askedRegion = -1; m.TeleportHeroToRegionTeleporter(1);
    bad |= (m.m_teleports != 1 || m.m_askedRegion != -1);
    g_hero.m_flags = 0; g_noTeleporter = true; m.TeleportHeroToRegionTeleporter(1);
    bad |= (m.m_teleports != 1);
    if (bad) { std::printf("005fc840_TEST FAIL\n"); return 1; }
    std::printf("005fc840_TEST PASS\n");
    return 0;
}