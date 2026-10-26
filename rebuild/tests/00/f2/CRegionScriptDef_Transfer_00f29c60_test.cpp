// behaviour test for 0x00F29C60 CRegionScriptDef::Transfer: RandomVillagerMax / RandomGuardMax / RandomBanditMax / RegionDangerLevel are transferred in order under their own names with zero defaults, so a reset context restores the defaults and a save/load round trip keeps the values
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
struct Script { long m0; long m1; long m2; long m3; };
static void Persist(Script& s, Context& c)                   // the function under test
{
    c.Transfer("RandomVillagerMax", s.m0, (long)(0L));
    c.Transfer("RandomGuardMax", s.m1, (long)(0L));
    c.Transfer("RandomBanditMax", s.m2, (long)(0L));
    c.Transfer("RegionDangerLevel", s.m3, (long)(0L));
}
int main()
{
    int fail = 0;
    Script s; s.m0 = (long)1; s.m1 = (long)2; s.m2 = (long)3; s.m3 = (long)4;
    Context save; save.mode = 0; Persist(s, save);
    if (save.order != std::string("RandomVillagerMax;RandomGuardMax;RandomBanditMax;RegionDangerLevel;")) fail |= 1;
    Script r; r.m0 = (long)0; r.m1 = (long)0; r.m2 = (long)0; r.m3 = (long)0;
    Context load; load.mode = 1; load.store = save.store; Persist(r, load);
    if (r.m0 != s.m0 || r.m1 != s.m1 || r.m2 != s.m2 || r.m3 != s.m3) fail |= 2;
    Context reset; reset.mode = 2; Persist(r, reset);
    if (r.m0 != (long)(0L) || r.m1 != (long)(0L) || r.m2 != (long)(0L) || r.m3 != (long)(0L)) fail |= 4;
    if (fail) { std::printf("00f29c60_TEST FAIL %d\n", fail); return 1; }
    std::printf("00f29c60_TEST PASS\n");
    return 0;
}