// behaviour test for 0x00DCC720 NScript::CQ_OrchardFarmRaidScript::OnPersist: ShownCombatMultiplierTutorial / HeroMetWhisperBeforeFarm are transferred in order under their own names with their defaults, so a reset context restores the defaults and a save/load round trip keeps the values
#include <cstdio>
#include <map>
#include <string>
struct Context {
    int mode;                                                // 0 save, 1 load, 2 reset
    std::map<std::string, double> store; std::string order;
    template <class T> void Transfer(const char* name, T& value, const T& def)
    {
        order += name; order += ";";
        if (mode == 0) store[name] = (double)value;
        else if (mode == 1) value = (T)store[name];
        else value = def;
    }
};
struct Script { bool m0; bool m1; };
static void Persist(Script& s, Context& c)                   // the function under test
{
    c.Transfer("ShownCombatMultiplierTutorial", s.m0, (bool)(false));
    c.Transfer("HeroMetWhisperBeforeFarm", s.m1, (bool)(false));
}
int main()
{
    int fail = 0;
    Script s; s.m0 = (bool)1; s.m1 = (bool)1;
    Context save; save.mode = 0; Persist(s, save);
    if (save.order != std::string("ShownCombatMultiplierTutorial;HeroMetWhisperBeforeFarm;")) fail |= 1;
    Script r; r.m0 = (bool)0; r.m1 = (bool)0;
    Context load; load.mode = 1; load.store = save.store; Persist(r, load);
    if (r.m0 != s.m0 || r.m1 != s.m1) fail |= 2;
    Context reset; reset.mode = 2; Persist(r, reset);
    if (r.m0 != (bool)(false) || r.m1 != (bool)(false)) fail |= 4;
    if (fail) { std::printf("00dcc720_TEST FAIL %d\n", fail); return 1; }
    std::printf("00dcc720_TEST PASS\n");
    return 0;
}