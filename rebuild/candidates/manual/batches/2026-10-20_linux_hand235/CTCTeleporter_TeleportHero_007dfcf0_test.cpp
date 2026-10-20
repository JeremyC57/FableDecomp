// behaviour test for 007dfcf0 CTCTeleporter::TeleportHero: marks the teleport region load, then hands the world event
// 0x2c with the payload X, Y, Z, facing (floats), true, false, false and the main player number
#include <cstdio>
#include <cstring>
class CThing;
struct C3DVector { float X, Y, Z; };
struct CTCPhysicsBase {
    virtual float GetFacingAngleXY() { return 0.25f; }
};
class CThing {
public:
    C3DVector       Pos;
    CTCPhysicsBase* PhysicsTC;
    const C3DVector& GetPosition() const { return Pos; }
};
static unsigned char g_payload[64];
static unsigned long g_payloadLength = 0;
struct CMemoryBuffer {
    void Copy(unsigned long length, const void* data) { std::memcpy(g_payload + g_payloadLength, data, length); g_payloadLength += length; }
};
struct CGameEvent : CMemoryBuffer {
    long          Type;
    signed char   Player;
    unsigned char Data[32];
    unsigned char EndPos;
    bool          Valid;
    bool          Replacement;
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddFloat(float value) { Copy(4, &value); }
    void AddBool(bool value) { Copy(1, &value); }
};
static long g_eventType = 0; static int g_eventPlayer = 0; static bool g_flagAtEvent = false;
struct CWorld {
    bool RegionLoadViaTeleporter;
    void HandleMoveHeroToRegionGameEvent(CGameEvent* event) { g_eventType = event->Type; g_eventPlayer = event->Player; g_flagAtEvent = RegionLoadViaTeleporter; }
};
struct CPlayerManager {
    long GetMainPlayerNumber() { return 3; }
};
static CWorld g_world; static CPlayerManager g_players;
struct CTCBase {
    virtual void v00() {}
    CThing* ParentThing;
    CWorld* GetWorld() { return &g_world; }
    CPlayerManager* GetPlayerManager() const { return &g_players; }
};
struct CTCTeleporter : CTCBase {
    void TeleportHero();
};
void CTCTeleporter::TeleportHero()
{
    GetWorld()->RegionLoadViaTeleporter = true;
    CGameEvent event(0x2c);
    event.AddFloat(ParentThing->GetPosition().X);
    event.AddFloat(ParentThing->GetPosition().Y);
    event.AddFloat(ParentThing->GetPosition().Z);
    event.AddFloat(ParentThing->PhysicsTC->GetFacingAngleXY());
    event.AddBool(true);
    event.AddBool(false);
    event.AddBool(false);
    event.Player = (signed char)GetPlayerManager()->GetMainPlayerNumber();
    GetWorld()->HandleMoveHeroToRegionGameEvent(&event);
}
int main()
{
    CTCPhysicsBase physics; CThing thing; thing.Pos.X = 1.0f; thing.Pos.Y = 2.0f; thing.Pos.Z = 3.0f; thing.PhysicsTC = &physics;
    CTCTeleporter teleporter; teleporter.ParentThing = &thing; g_world.RegionLoadViaTeleporter = false;
    teleporter.TeleportHero();
    float f[4]; std::memcpy(f, g_payload, 16);
    bool ok = g_eventType == 0x2c && g_eventPlayer == 3 && g_flagAtEvent && g_payloadLength == 19 && f[0] == 1.0f &&
              f[1] == 2.0f && f[2] == 3.0f && f[3] == 0.25f && g_payload[16] == 1 && g_payload[17] == 0 && g_payload[18] == 0;
    if (!ok) { std::printf("007dfcf0_TEST FAIL\n"); return 1; }
    std::printf("007dfcf0_TEST PASS\n");
    return 0;
}
