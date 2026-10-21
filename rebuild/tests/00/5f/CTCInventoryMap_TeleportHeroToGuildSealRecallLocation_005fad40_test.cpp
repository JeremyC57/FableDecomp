#include <cstdio>
#include <cstring>
// Behaviour test for CTCInventoryMap::TeleportHeroToGuildSealRecallLocation (0x005FAD40): same body as src/, with the
// player lookups, world access, buffer copy and the event handler stubbed and recorded.
struct C3DVector { float X, Y, Z; };
struct CMemoryBuffer {
    unsigned char buf[64]; int len;
    CMemoryBuffer() : len(0) { }
    void Copy(unsigned long length, const void* data) { std::memcpy(buf + len, data, length); len += (int)length; }
};
struct CGameEvent : CMemoryBuffer {
    long          Type;
    signed char   Player;
    unsigned char Data[32];
    unsigned char EndPos;
    bool          Valid;
    bool          Replacement;
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddFloat(float value) { Copy(sizeof(value), &value); }
    void AddBool(bool value) { Copy(1, &value); }
};
struct CThing { unsigned char IsDead : 1; };
static CThing* g_hero = 0;
struct CPlayer { CThing* GetControlledCreature() const { return g_hero; } };
static CPlayer g_player;
struct CPlayerManager {
    CPlayer* GetMainPlayer() const { return &g_player; }
    long GetMainPlayerID() const { return 0x103; }      // truncated to the event's signed char
};
static CPlayerManager g_pm;
struct CEngine { CPlayerManager* PlayerManager; };
static CEngine g_engine = { &g_pm };
static CEngine* g_pEngine = &g_engine;
static int g_events = 0; static long g_type = 0; static signed char g_playerId = 0;
static unsigned char g_payload[64]; static int g_payloadLen = 0;
struct CWorld {
    bool DisplayMiniMapOnRegionLoad;
    C3DVector GuildSealRecallPos;
    float GuildSealRecallAngleXY;
    void HandleMoveHeroToRegionGameEvent(const CGameEvent* e)
    { ++g_events; g_type = e->Type; g_playerId = e->Player; g_payloadLen = e->len; std::memcpy(g_payload, e->buf, e->len); }
};
static CWorld g_world;
struct CTCInventoryMap {
    CPlayerManager* GetPlayerManager() const { return &g_pm; }
    CWorld* GetWorld() { return &g_world; }
    const CWorld* GetWorld() const { return &g_world; }
    const CWorld& GetWorldConst() const { return *GetWorld(); }
    void TeleportHeroToGuildSealRecallLocation();
};
void CTCInventoryMap::TeleportHeroToGuildSealRecallLocation()
{
    CThing* hero = GetPlayerManager()->GetMainPlayer()->GetControlledCreature();
    if (hero && !hero->IsDead) {
        GetWorld()->DisplayMiniMapOnRegionLoad = true;
        const C3DVector& pos = GetWorldConst().GuildSealRecallPos;
        float angle = GetWorldConst().GuildSealRecallAngleXY;
        CGameEvent event(0x2c);
        event.AddFloat(pos.X);
        event.AddFloat(pos.Y);
        event.AddFloat(pos.Z);
        event.AddFloat(angle);
        event.AddBool(true);
        event.AddBool(false);
        event.AddBool(false);
        event.Player = (signed char)g_pEngine->PlayerManager->GetMainPlayerID();
        GetWorld()->HandleMoveHeroToRegionGameEvent(&event);
    }
}

int main()
{
    int fails = 0;
    CTCInventoryMap map;
    g_world.DisplayMiniMapOnRegionLoad = false;
    g_world.GuildSealRecallPos.X = 1.5f; g_world.GuildSealRecallPos.Y = -2.0f; g_world.GuildSealRecallPos.Z = 3.25f;
    g_world.GuildSealRecallAngleXY = 0.75f;
    map.TeleportHeroToGuildSealRecallLocation();                   // no hero
    if (g_events || g_world.DisplayMiniMapOnRegionLoad) ++fails;
    CThing hero; hero.IsDead = 1; g_hero = &hero;
    map.TeleportHeroToGuildSealRecallLocation();                   // dead hero
    if (g_events || g_world.DisplayMiniMapOnRegionLoad) ++fails;
    hero.IsDead = 0;
    map.TeleportHeroToGuildSealRecallLocation();
    if (g_events != 1 || !g_world.DisplayMiniMapOnRegionLoad || g_type != 0x2c || g_playerId != 3) ++fails;
    float f[4]; std::memcpy(f, g_payload, 16);
    if (g_payloadLen != 19 || f[0] != 1.5f || f[1] != -2.0f || f[2] != 3.25f || f[3] != 0.75f) ++fails;
    if (g_payload[16] != 1 || g_payload[17] != 0 || g_payload[18] != 0) ++fails;
    if (fails) { std::printf("005fad40_TEST FAIL %d\n", fails); return 1; }
    std::printf("005fad40_TEST PASS\n");
    return 0;
}