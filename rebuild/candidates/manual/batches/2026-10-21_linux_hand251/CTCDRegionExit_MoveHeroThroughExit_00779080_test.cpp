#include <cstdio>
// behaviour test: with a live hero the exit flags the world and hands it one 0x2c event holding the entrance's X, Y, Z and heading
// plus three false flags, stamped with the hero's player; a missing or dead hero does nothing
#include <cstring>
struct C3DVector { float X, Y, Z; };
struct CThing { C3DVector Pos; float Angle; signed char Player; bool Dead; };
struct CGameEvent {
    long Type; signed char Player; unsigned char Data[32]; unsigned char EndPos;
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0) { }
    void Copy(unsigned long n, const void* p) { std::memcpy(Data + EndPos, p, n); EndPos = (unsigned char)(EndPos + n); }
    void AddFLOAT(float v) { Copy(4, &v); }
    void AddBOOL(bool v) { Copy(1, &v); }
};
struct CWorld { bool HeroMovingThroughRegionExit; int events; CGameEvent last; CWorld() : HeroMovingThroughRegionExit(false), events(0), last(0) { }
    void Handle(const CGameEvent& e) { ++events; last = e; } };
static CWorld g_world;
static CThing* g_hero = 0;
struct CTCDRegionExit {
    CThing* PEntranceConnectedTo;
    void MoveHeroThroughExit()
    {
        CThing* hero = g_hero;
        if (hero && !hero->Dead) {
            g_world.HeroMovingThroughRegionExit = true;
            CGameEvent event(0x2c);
            event.AddFLOAT(PEntranceConnectedTo->Pos.X);
            event.AddFLOAT(PEntranceConnectedTo->Pos.Y);
            event.AddFLOAT(PEntranceConnectedTo->Pos.Z);
            event.AddFLOAT(PEntranceConnectedTo->Angle);
            event.AddBOOL(false);
            event.AddBOOL(false);
            event.AddBOOL(false);
            event.Player = hero->Player;
            g_world.Handle(event);
        }
    }
};
int main()
{
    CThing entrance = { { 1.0f, 2.0f, 3.0f }, 0.5f, 0, false };
    CThing hero = { { 0, 0, 0 }, 0, 2, true };
    CTCDRegionExit exitTC = { &entrance };
    exitTC.MoveHeroThroughExit();
    bool ok = g_world.events == 0;
    g_hero = &hero; exitTC.MoveHeroThroughExit();
    ok = ok && g_world.events == 0 && !g_world.HeroMovingThroughRegionExit;
    hero.Dead = false; exitTC.MoveHeroThroughExit();
    float v[4]; std::memcpy(v, g_world.last.Data, 16);
    ok = ok && g_world.events == 1 && g_world.HeroMovingThroughRegionExit && g_world.last.Type == 0x2c && g_world.last.Player == 2 &&
         g_world.last.EndPos == 19 && v[0] == 1.0f && v[2] == 3.0f && v[3] == 0.5f && g_world.last.Data[16] == 0;
    if (!ok) { std::printf("00779080_TEST FAIL\n"); return 1; }
    std::printf("00779080_TEST PASS\n");
    return 0;
}
