#include <cstdio>
// Host model of CGlobal_WatchForHeroDeathScript::WatchForHeroDeath: pump while the hero is valid and alive, then pump
// and look again, until the script thread terminates.
struct Hero { bool valid, dead; bool IsValid() const { return valid; } bool IsDead() const { return dead; } };
struct Interface {
    Hero hero; int pumps; int killAt; int reviveAt;
    const Hero& GetHero() const { return hero; }
    void Pump() { ++pumps; if (pumps == killAt) hero.dead = true; if (pumps == reviveAt) hero.dead = false; }
};
struct Script {
    Interface* itf; int terminateAfter;
    bool IsActiveThreadTerminating() const { return itf->pumps >= terminateAfter; }
    void WatchForHeroDeath()
    {
        while (!IsActiveThreadTerminating()) {
            while (itf->GetHero().IsValid() && !itf->GetHero().IsDead()) {
                itf->Pump();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            itf->Pump();
        }
    }
};
int main()
{
    int fail = 0;
    Interface i; i.hero.valid = true; i.hero.dead = false; i.pumps = 0; i.killAt = 3; i.reviveAt = 6;
    Script s; s.itf = &i; s.terminateAfter = 10;
    s.WatchForHeroDeath();
    if (i.pumps != 10) fail = 1;
    Interface j; j.hero.valid = false; j.hero.dead = false; j.pumps = 0; j.killAt = -1; j.reviveAt = -1;
    Script t; t.itf = &j; t.terminateAfter = 0;
    t.WatchForHeroDeath();
    if (j.pumps != 0) fail = 1;
    if (fail) { std::printf("00ee8fe0_TEST FAIL\n"); return 1; }
    std::printf("00ee8fe0_TEST PASS\n");
    return 0;
}