#include <cstdio>
#include <cstring>
// behaviour test: CTCHeroCentreDoorMarker::FrameUpdate raises event 0x34 once when the hero enters the radius,
// clears the inside flag when the hero leaves or is gone, and runs the particle update only for door type 1
struct C3DVector { float X, Y, Z; };
struct CThing {
    unsigned long long UID;
    signed char Player;
    bool Dead;
    C3DVector Pos;
    const C3DVector& GetPosition() const { return Pos; }
};
struct CIntelligentPointer { CThing* p; CThing* GetPItem() const { return p; } };
static bool IS_THING_ALIVE(const CIntelligentPointer& p) { return p.GetPItem() && !p.GetPItem()->Dead; }
static CThing* g_hero = 0;
struct CPlayer { CThing* GetHeroThing() { return g_hero; } };
struct CPlayerManager { CPlayer player; CPlayer* GetMainPlayer() { return &player; } };
static CPlayerManager g_pm;
struct CGameEvent {
    long Type; signed char Player; unsigned char Data[32]; unsigned char EndPos;
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0) {}
    void Copy(unsigned long n, const void* d) { std::memcpy(Data + EndPos, d, n); EndPos = (unsigned char)(EndPos + n); }
    void AddLONG(long v) { Copy(4, &v); }
    void AddUID(unsigned long long v) { Copy(8, &v); }
};
static int g_events = 0; static CGameEvent g_last(0);
static void AddGameEvent(const CGameEvent& e) { ++g_events; g_last = e; }
struct CTCHeroCentreDoorMarker {
    CThing* ParentThing; float Radius; long DoorType; bool HeroInsideRadius;
    CIntelligentPointer PHeroCentre; int particleUpdates;
    CPlayerManager* GetPlayerManager() { return &g_pm; }
    void UpdateParticleExperience() { ++particleUpdates; }
    void FrameUpdate()
    {
        if (IS_THING_ALIVE(PHeroCentre)) {
            CThing* hero = GetPlayerManager()->GetMainPlayer()->GetHeroThing();
            if (hero && !hero->Dead) {
                const C3DVector& pos = ParentThing->GetPosition();
                const C3DVector& heroPos = hero->GetPosition();
                float dx = heroPos.X - pos.X;
                float dy = heroPos.Y - pos.Y;
                float radius = Radius;
                if (dx * dx + dy * dy < radius * radius) {
                    if (!HeroInsideRadius) {
                        CGameEvent event(0x34);
                        event.AddLONG(DoorType);
                        event.AddUID(PHeroCentre.GetPItem()->UID);
                        event.Player = hero->Player;
                        AddGameEvent(event);
                        HeroInsideRadius = true;
                    }
                } else {
                    HeroInsideRadius = false;
                }
            } else {
                HeroInsideRadius = false;
            }
        }
        if (DoorType == 1)
            UpdateParticleExperience();
    }
};
int main()
{
    CThing me = {0, 0, false, {0, 0, 0}};
    CThing centre = {0x1122334455667788ULL, 0, false, {0, 0, 0}};
    CThing hero = {0, 3, false, {3, 4, 100}};
    g_hero = &hero;
    CTCHeroCentreDoorMarker m; m.ParentThing = &me; m.Radius = 6.0f; m.DoorType = 1; m.HeroInsideRadius = false;
    m.PHeroCentre.p = &centre; m.particleUpdates = 0;
    m.FrameUpdate();   // dist 5 < 6, Z ignored
    bool ok = g_events == 1 && m.HeroInsideRadius && g_last.Type == 0x34 && g_last.Player == 3 && g_last.EndPos == 12
        && *(int*)g_last.Data == 1 && *(unsigned long long*)(g_last.Data + 4) == 0x1122334455667788ULL && m.particleUpdates == 1;
    m.FrameUpdate();   // already inside: no new event
    ok = ok && g_events == 1 && m.HeroInsideRadius && m.particleUpdates == 2;
    hero.Pos.X = 10;   // outside
    m.FrameUpdate();
    ok = ok && g_events == 1 && !m.HeroInsideRadius;
    hero.Pos.X = 3; m.DoorType = 2; hero.Dead = true; m.HeroInsideRadius = true;
    m.FrameUpdate();   // dead hero clears the flag, door type 2 skips the particles
    ok = ok && !m.HeroInsideRadius && m.particleUpdates == 3;
    centre.Dead = true; m.HeroInsideRadius = true; hero.Dead = false;
    m.FrameUpdate();   // dead centre: nothing changes
    ok = ok && m.HeroInsideRadius && g_events == 1;
    if (!ok) { std::printf("0077fb00_TEST FAIL\n"); return 1; }
    std::printf("0077fb00_TEST PASS\n");
    return 0;
}
