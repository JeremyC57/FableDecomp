// behaviour test for 0x00D44C60 NScript::CQ_GuildTrainingScript::CBirdKiller::OnPersist: BirdMode / CurrentBirds are transferred in order under their own names with their defaults, so a reset context restores the defaults and a save/load round trip keeps the values
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
struct Script { long m0; long m1; };
static void Persist(Script& s, Context& c)                   // the function under test
{
    c.Transfer("BirdMode", s.m0, (long)(0L));
    c.Transfer("CurrentBirds", s.m1, (long)(0L));
}
int main()
{
    int fail = 0;
    Script s; s.m0 = (long)1; s.m1 = (long)2;
    Context save; save.mode = 0; Persist(s, save);
    if (save.order != std::string("BirdMode;CurrentBirds;")) fail |= 1;
    Script r; r.m0 = (long)0; r.m1 = (long)0;
    Context load; load.mode = 1; load.store = save.store; Persist(r, load);
    if (r.m0 != s.m0 || r.m1 != s.m1) fail |= 2;
    Context reset; reset.mode = 2; Persist(r, reset);
    if (r.m0 != (long)(0L) || r.m1 != (long)(0L)) fail |= 4;
    if (fail) { std::printf("00d44c60_TEST FAIL %d\n", fail); return 1; }
    std::printf("00d44c60_TEST PASS\n");
    return 0;
}