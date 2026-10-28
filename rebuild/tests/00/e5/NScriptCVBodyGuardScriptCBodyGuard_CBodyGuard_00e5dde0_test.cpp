// Test: CV_BodyGuardScript::CBodyGuard ctor -- entity-script base over the parent's Game and Me, ParentClass, the two timers built, then PMasterData from the script data.
#include <cstdio>
static int g_timers = 0;
class CThing { public: int id; };
class CTimer { public: int h; CTimer() : h(-1) { ++g_timers; } };
struct CEntity { void* Game; CThing Me; CEntity(const CThing& me, void* game) : Game(game), Me(me) { } };
struct CParent { void* Game; };
struct CGuard : CEntity { CParent* ParentClass; void* PMasterData; CTimer AngerTimer; CTimer AttackedBySummonersTimer;
    CGuard(CParent& p, void* data, const CThing& me) : CEntity(me, p.Game), ParentClass(&p) { PMasterData = data; } };
int main()
{
    int game, data; CParent p; p.Game = &game; CThing me; me.id = 42;
    CGuard g(p, &data, me);
    bool ok = g.Game == &game && g.Me.id == 42 && g.ParentClass == &p && g.PMasterData == &data && g_timers == 2 && g.AngerTimer.h == -1;
    std::printf(ok ? "00e5dde0_TEST PASS\n" : "00e5dde0_TEST FAIL\n");
    return ok ? 0 : 1;
}