#include <cstdio>
// Model of CUserProfileManager::GetMaxEffectsDetail: a command-line override (-1 = unset) wins; else the
// CONFIG_OPTIONS_DEFAULTS_DEF field; 0.0 when no definition resolves.
struct Def { long MaxXDetail; };
static long g_MaxDetail = -1;
static const Def* g_def;
static const Def* GetConfigOptionsDefaultsDef() { return g_def; }
static float GetMaxDetail()
{
    if (g_MaxDetail != -1) return (float)g_MaxDetail;
    const Def* def = GetConfigOptionsDefaultsDef();
    if (def) return (float)def->MaxXDetail;
    return 0.0f;
}
int main()
{
    Def d = { 3 };
    if (GetMaxDetail() != 0.0f) { std::printf("004084b0_TEST FAIL\n"); return 1; }
    g_def = &d;
    if (GetMaxDetail() != 3.0f) { std::printf("004084b0_TEST FAIL\n"); return 1; }
    g_MaxDetail = 1;
    if (GetMaxDetail() != 1.0f) { std::printf("004084b0_TEST FAIL\n"); return 1; }
    std::printf("004084b0_TEST PASS\n"); return 0;
}