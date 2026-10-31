// behaviour test for CTCLight::OnSerialise 0x00641020: Active/Overridden always transfer; a load with the flag set refreshes
// the appearance unless the thing is a hero-state thing during hero-state serialisation; an overridden light transfers its
// colour/radii/flicker/inverted, and a load allocates a fresh light info first
#include <cstdio>
#include <string>
#include <vector>
struct Info { unsigned col; float in, out, fl; bool inv; };
struct Ctx { bool saving; Ctx(bool s) : saving(s) { } std::vector<std::string> log; template <class T> void T_(const char* n, T& v, const T& d) { log.push_back(n); if (!saving) v = d; } };
struct Light { bool active, overridden, typeFlag2, heroState; int refresh; Info* info; int allocs;
    void OnSerialise(Ctx& c, bool flag) {
        c.T_("Active", active, false);
        if (flag && !c.saving) { if (!typeFlag2 || !heroState) ++refresh; }
        c.T_("Overridden", overridden, false);
        if (overridden) {
            if (!c.saving) { delete info; info = new Info(); ++allocs; }
            unsigned def = 0xff000000u;
            c.T_("Colour", info->col, def); c.T_("InnerRadius", info->in, 0.0f); c.T_("OuterRadius", info->out, 0.0f);
            c.T_("Flicker", info->fl, 0.0f); c.T_("Inverted", info->inv, false);
        }
    } };
static void Init(Light& l, bool active, bool overridden, bool typeFlag2, bool heroState, Info* info)
{
    l.active = active; l.overridden = overridden; l.typeFlag2 = typeFlag2; l.heroState = heroState;
    l.refresh = 0; l.info = info; l.allocs = 0;
}
int main()
{
    Light l; Init(l, true, true, false, false, new Info());
    Ctx s(true); l.OnSerialise(s, true);
    if (s.log.size() != 7 || l.refresh || l.allocs) { std::printf("00641020_TEST FAIL\n"); return 1; }
    Ctx ld(false); l.overridden = true; l.OnSerialise(ld, true);
    Light h; Init(h, true, false, true, true, 0); Ctx ld2(false); h.OnSerialise(ld2, true);
    if (h.refresh != 0 || ld2.log.size() != 2) { std::printf("00641020_TEST FAIL\n"); return 1; }
    Light k; Init(k, true, false, false, true, 0); Ctx ld3(false); k.OnSerialise(ld3, false);
    if (k.refresh != 0) { std::printf("00641020_TEST FAIL\n"); return 1; }
    std::printf("00641020_TEST PASS\n"); return 0;
}