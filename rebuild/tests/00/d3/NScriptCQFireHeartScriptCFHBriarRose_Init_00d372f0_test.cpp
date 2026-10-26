// behaviour test for 0x00D372F0 NScript::CQ_FireHeartScript::CFH_BriarRose::Init(): exactly one interface call, EntitySetAsKillable(Me, false, true), on the script's own Game with its own Me
#include <cstdio>
struct Thing { int Id; };
struct Interface {
    int Calls, Which, A, B; const Thing* Arg;
    Interface() : Calls(0), Which(0), A(-1), B(-1), Arg(0) { }
    virtual void RemoveThing(const Thing& thing, bool flag, bool byScript) { ++Calls; Which = 108; Arg = &thing; A = flag; B = byScript; }
    virtual void EntitySetAsKillable(const Thing& thing, bool killable, bool flag) { ++Calls; Which = 517; Arg = &thing; A = killable; B = flag; }
    virtual ~Interface() { }
};
struct Script {
    Interface* Game;
    Thing Me;
    void Run() { Game->EntitySetAsKillable(Me, false, true); }   // the function under test
};
int main()
{
    int fail = 0;
    Interface game, other;
    Script s; s.Game = &game; s.Me.Id = 7;
    s.Run();
    if (game.Calls != 1 || game.Which != 517) fail |= 1;
    if (game.Arg != &s.Me || game.Arg->Id != 7) fail |= 2;
    if (game.A != 0 || game.B != 1) fail |= 4;
    if (other.Calls != 0) fail |= 8;
    if (fail) { std::printf("00d372f0_TEST FAIL %d\n", fail); return 1; }
    std::printf("00d372f0_TEST PASS\n");
    return 0;
}