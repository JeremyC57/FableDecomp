// Behaviour test for 0x00890090 CGameScriptInterface::SetEnvironmentThemeWeightExternals: a host model of the row's logic --
// look the theme up by name; only a positive global index reaches CEnvironment::SetThemeWeight(0, 1, theme, weight).
#include <cstdio>
#include <cstring>

struct CCharString { const char* Text; };
struct CEnvironment {
    int calls; long channel, priority, theme; float weight;
    void SetThemeWeight(long c, long p, long t, float w) { ++calls; channel = c; priority = p; theme = t; weight = w; }
};
struct CWorld { char pad[0x18]; CEnvironment* PEnvironment_ptr; };
struct CDefinitionManager {
    long GetDefGlobalIndexFromName(const CCharString& n) const {
        if (std::strcmp(n.Text, "THEME_SUNNY") == 0) return 12;
        if (std::strcmp(n.Text, "THEME_ZERO") == 0) return 0;
        return -1;
    }
};
static CDefinitionManager g_defs;
struct CGameDefinitionManager { static const CDefinitionManager& Get() { return g_defs; } };
struct CGameScriptInterface {
    CWorld* World;
    void SetEnvironmentThemeWeightExternals(const CCharString& theme_name, float weight) const
    {
        long theme = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(theme_name);
        if (theme > 0)
            World->PEnvironment_ptr->SetThemeWeight(0, 1, theme, weight);
    }
};

int main()
{
    CEnvironment env = {0, 0, 0, 0, 0.0f};
    CWorld world; world.PEnvironment_ptr = &env;
    CGameScriptInterface gsi; gsi.World = &world;
    CCharString missing = {"NOPE"}, zero = {"THEME_ZERO"}, sunny = {"THEME_SUNNY"};
    gsi.SetEnvironmentThemeWeightExternals(missing, 0.5f);
    gsi.SetEnvironmentThemeWeightExternals(zero, 0.5f);
    if (env.calls != 0) { std::printf("FAIL non-positive index set a weight\n"); return 1; }
    gsi.SetEnvironmentThemeWeightExternals(sunny, 0.75f);
    if (env.calls != 1 || env.channel != 0 || env.priority != 1 || env.theme != 12 || env.weight != 0.75f) {
        std::printf("FAIL weight call\n"); return 1;
    }
    std::printf("00890090_TEST PASS\n");
    return 0;
}
