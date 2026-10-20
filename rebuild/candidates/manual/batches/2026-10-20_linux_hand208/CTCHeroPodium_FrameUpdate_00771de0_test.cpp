// Behaviour test for CTCHeroPodium::FrameUpdate (0x00771DE0): stepping within the radius posts the type-0x23
// message once (resetting the crowd), leaving removes it; no controlled creature does nothing.
#include <cstdio>

struct C3DVector { float X, Y, Z; };
const float g_podiumRadiusSquared_0126a88c = 1.0f;
struct CThing { C3DVector Pos; const C3DVector& GetPosition() const { return Pos; } };
struct CPlayer { CThing* Creature; CThing* GetControlledCreature() { return Creature; } };
struct CMessageEvent { long Type; };
struct CMessageEventFilter_IsOfType { long Type; CMessageEventFilter_IsOfType(long type) : Type(type) {} };
static CMessageEvent g_event; static bool g_posted = false; static int g_adds = 0, g_removes = 0; static CThing* g_from = 0; static unsigned long g_delay = 0;
struct CMessageEventManager {
    CMessageEvent* FindMostRecentMessage(const CMessageEventFilter_IsOfType& f) { return (g_posted && f.Type == 0x23) ? &g_event : 0; }
    CMessageEvent* AddMessage(long type, unsigned long delay, const C3DVector&, CThing* from, CThing*, float, float, long)
    { g_posted = true; g_event.Type = type; g_delay = delay; g_from = from; ++g_adds; return &g_event; }
    void RemoveMessage(const CMessageEvent*) { g_posted = false; ++g_removes; }
};
struct CWorld { CMessageEventManager* PMessageEventManager; CPlayer* Player; CPlayer* GetCurrentPlayer() { return Player; } };
static int g_resizes = 0, g_clears = 0;
struct CLongVector { void resize(unsigned long n, const long& v) { if (n == 8 && v == 0) ++g_resizes; } void clear() { ++g_clears; } };
static CWorld g_world;
struct CTCBase { CThing* ParentThing; CWorld* GetWorld() { return &g_world; } };
struct CTCHeroPodium : CTCBase {
    long NumberOfCrowdOccupants; CLongVector CrowdOccupation;
    void FrameUpdate();
};
void CTCHeroPodium::FrameUpdate()
{
    CThing* podium = ParentThing;
    const C3DVector& podiumPos = podium->GetPosition();
    CThing* hero = GetWorld()->GetCurrentPlayer()->GetControlledCreature();
    if (hero) {
        const C3DVector& heroPos = hero->GetPosition();
        float dx = podiumPos.X - heroPos.X;
        float dy = podiumPos.Y - heroPos.Y;
        bool onPodium = dx * dx + dy * dy < g_podiumRadiusSquared_0126a88c ? true : false;
        CMessageEventManager* messages = GetWorld()->PMessageEventManager;
        CMessageEvent* message = messages->FindMostRecentMessage(CMessageEventFilter_IsOfType(0x23));
        if (onPodium) {
            if (message == 0) {
                CrowdOccupation.resize(8, 0);
                NumberOfCrowdOccupants = 0;
                messages->AddMessage(0x23, 0x249F0, podiumPos, podium, 0, 0.0f, 0.0f, 0);
            }
        } else if (message) {
            messages->RemoveMessage(message);
            CrowdOccupation.clear();
        }
    }
}
int main()
{
    CMessageEventManager mgr; CPlayer player; CThing podium, hero;
    podium.Pos.X = 10.0f; podium.Pos.Y = 10.0f; podium.Pos.Z = 0.0f;
    hero.Pos.X = 20.0f; hero.Pos.Y = 10.0f; hero.Pos.Z = 99.0f;
    player.Creature = 0; g_world.Player = &player; g_world.PMessageEventManager = &mgr;
    CTCHeroPodium p; p.ParentThing = &podium; p.NumberOfCrowdOccupants = 5;
    p.FrameUpdate();
    player.Creature = &hero; p.FrameUpdate();
    if (g_adds || g_removes) { std::printf("00771de0_TEST FAIL\n"); return 1; }
    hero.Pos.X = 10.5f; hero.Pos.Y = 10.5f; p.FrameUpdate(); p.FrameUpdate();
    if (g_adds != 1 || g_resizes != 1 || p.NumberOfCrowdOccupants != 0 || g_from != &podium || g_delay != 0x249F0 || g_event.Type != 0x23) { std::printf("00771de0_TEST FAIL\n"); return 1; }
    hero.Pos.X = 11.0f; hero.Pos.Y = 11.0f; p.FrameUpdate(); p.FrameUpdate();   // dx*dx+dy*dy == 2 >= 1
    if (g_removes != 1 || g_clears != 1 || g_posted) { std::printf("00771de0_TEST FAIL\n"); return 1; }
    std::printf("00771de0_TEST PASS\n");
    return 0;
}
