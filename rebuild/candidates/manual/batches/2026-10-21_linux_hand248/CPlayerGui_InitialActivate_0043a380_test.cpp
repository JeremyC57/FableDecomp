// Behaviour test for 0043a380 (CPlayerGui::InitialActivate): the subtitles and quest info are cleared, all five stat
// bars hidden, both alpha arrays sized to the GUI def's ability-button count and filled with 1.0, the stat-update
// flag cleared and OldDrawGui set; the def is loaded once.
#include <cstdio>

static int g_defLoads = 0;
struct Def { unsigned long buttons; };
static Def* g_def = 0;
static Def g_theDef = { 6 };
static const Def* GetDef() { if (!g_def) { ++g_defLoads; g_def = &g_theDef; } return g_def; }
struct Bar { bool Active; };
struct FloatVec { float v[16]; unsigned long n; void resize(unsigned long count, const float& x) { for (unsigned long i = n; i < count; ++i) v[i] = x; n = count; } };
struct Gui {
    int subtitleClears, questClears; bool updating, oldDrawGui; Bar bars[5]; FloatVec alpha, quick;
    void InitialActivate()
    {
        ++subtitleClears; ++questClears;
        for (int i = 0; i < 5; ++i) bars[i].Active = false;
        alpha.resize(GetDef()->buttons, 1.0f);
        quick.resize(GetDef()->buttons, 1.0f);
        updating = false; oldDrawGui = true;
    }
};
int main()
{
    Gui g; g.subtitleClears = g.questClears = 0; g.updating = true; g.oldDrawGui = false;
    for (int i = 0; i < 5; ++i) g.bars[i].Active = true;
    g.alpha.n = 0; g.quick.n = 0;
    g.InitialActivate();
    int ok = g.subtitleClears == 1 && g.questClears == 1 && !g.updating && g.oldDrawGui && g.alpha.n == 6 && g.quick.n == 6 &&
             g.alpha.v[5] == 1.0f && g.quick.v[0] == 1.0f && g_defLoads == 1;
    for (int i = 0; i < 5; ++i) if (g.bars[i].Active) ok = 0;
    if (!ok) { std::printf("0043a380_TEST FAIL\n"); return 1; }
    std::printf("0043a380_TEST PASS\n");
    return 0;
}
