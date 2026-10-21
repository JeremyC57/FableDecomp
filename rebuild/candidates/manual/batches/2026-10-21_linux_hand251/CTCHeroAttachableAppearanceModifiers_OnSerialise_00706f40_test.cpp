#include <cstdio>
#include <string>
#include <vector>
// behaviour test: OnSerialise -- nothing unless first pass; saving (mode 1/3) writes the hair modifiers' def names; loading
// reads names back and re-adds each that resolves to an index > 0, then marks the modifiers loaded
static const char* g_Names[] = { "", "HAIR_A", "HAIR_B", "HAIR_C" };
static long IndexFromName(const std::string& n) { for (long i = 1; i < 4; ++i) if (n == g_Names[i]) return i; return 0; }
struct Ctx { long Mode; std::vector<std::string> Stored; };
struct Mods {
    std::vector<long> Hair; bool Loaded; std::vector<long> Added;
    void OnSerialise(Ctx& c, bool firstPass)
    {
        if (!firstPass) return;
        if (c.Mode == 1 || c.Mode == 3) {
            std::vector<std::string> names(Hair.size());
            for (unsigned long i = 0; i < Hair.size(); ++i) { std::string name = g_Names[Hair[i]]; names[i] = name; }
            c.Stored = names;
        } else {
            std::vector<std::string> names = c.Stored;
            for (unsigned long i = 0; i < names.size(); ++i) { long d = IndexFromName(names[i]); if (d > 0) Added.push_back(d); }
            Loaded = true;
        }
    }
};
int main()
{
    Mods m; m.Loaded = false; m.Hair.push_back(2); m.Hair.push_back(3);
    Ctx save = { 1, std::vector<std::string>() };
    m.OnSerialise(save, false);
    bool ok = save.Stored.empty();
    m.OnSerialise(save, true);
    ok = ok && save.Stored.size() == 2 && save.Stored[0] == "HAIR_B" && save.Stored[1] == "HAIR_C";
    Ctx load = { 2, save.Stored }; load.Stored.push_back("UNKNOWN");
    Mods n; n.Loaded = false;
    n.OnSerialise(load, true);
    ok = ok && n.Loaded && n.Added.size() == 2 && n.Added[0] == 2 && n.Added[1] == 3;
    if (!ok) { std::printf("00706f40_TEST FAIL\n"); return 1; }
    std::printf("00706f40_TEST PASS\n");
    return 0;
}
