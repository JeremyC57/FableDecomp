// Behaviour test for CUserProfileManager::ResetAssignedInputs (0x004085F0): scheme selection, manager selection, per-scheme def
// release before ApplyValues, and the vector copies.
#include <cstdio>
#include <cstring>
#include <vector>
#include <string>

struct Def { int refs; int deleted; std::vector<int> Controls; Def() : refs(0), deleted(0) {} };
struct DefPtr {
    Def* p;
    DefPtr() : p(0) {}
    ~DefPtr() { if (p && --p->refs == 0) p->deleted = 1; }
};
static Def g_lcik, g_two, g_agg, g_one;
static std::string g_log;
struct Mgr {
    const char* tag;
    bool GetDef(const char* name, DefPtr* out) const {
        Def* d = 0;
        if (!std::strcmp(name, "FABLE_PC_CONTROL_SCHEME_GDD_LEFT_CLICK_IS_KING")) d = &g_lcik;
        else if (!std::strcmp(name, "FABLE_PC_CONTROL_SCHEME_GDD")) d = &g_two;
        else if (!std::strcmp(name, "FABLE_PC_CONTROL_SCHEME_GDD_AGGRESSIVE")) d = &g_agg;
        else if (!std::strcmp(name, "FABLE_PC_CONTROL_SCHEME_GDD_ONE_MODE")) d = &g_one;
        g_log += tag;
        if (!d) return false;
        if (out->p != d) { if (out->p && --out->p->refs == 0) out->p->deleted = 1; out->p = d; ++d->refs; }
        return true;
    }
};
static Mgr g_game = { "G" }, g_startup = { "S" };
static bool g_bControlSchemeLeftClickIsKing; static unsigned char g_bAggressiveModeAvailable;
static void* g_main; static void* g_start;
static bool IsAllocated() { return g_start != 0; }   // CStartupDefinitionManager::IsAllocated (inline)

struct Profile {
    std::vector<int> AssignedInputsPassive, AssignedInputsAggressive;
    int applied; int liveRefsAtApply;
    Profile() : applied(0), liveRefsAtApply(-1) {}
    void ApplyValues() { ++applied; liveRefsAtApply = g_lcik.refs + g_two.refs + g_agg.refs + g_one.refs; }
    void ResetAssignedInputs()
    {
        if (g_bControlSchemeLeftClickIsKing) {
            DefPtr def;
            if (g_main) { g_game.GetDef("FABLE_PC_CONTROL_SCHEME_GDD_LEFT_CLICK_IS_KING", &def); AssignedInputsPassive = def.p->Controls; AssignedInputsAggressive = def.p->Controls; }
            else if (IsAllocated()) { g_startup.GetDef("FABLE_PC_CONTROL_SCHEME_GDD_LEFT_CLICK_IS_KING", &def); AssignedInputsPassive = def.p->Controls; AssignedInputsAggressive = def.p->Controls; }
        } else if (g_bAggressiveModeAvailable) {
            DefPtr def;
            if (g_main) { g_game.GetDef("FABLE_PC_CONTROL_SCHEME_GDD", &def); AssignedInputsPassive = def.p->Controls; g_game.GetDef("FABLE_PC_CONTROL_SCHEME_GDD_AGGRESSIVE", &def); AssignedInputsAggressive = def.p->Controls; }
            else if (IsAllocated()) { g_startup.GetDef("FABLE_PC_CONTROL_SCHEME_GDD", &def); AssignedInputsPassive = def.p->Controls; g_startup.GetDef("FABLE_PC_CONTROL_SCHEME_GDD_AGGRESSIVE", &def); AssignedInputsAggressive = def.p->Controls; }
        } else {
            DefPtr def;
            if (g_main) { g_game.GetDef("FABLE_PC_CONTROL_SCHEME_GDD_ONE_MODE", &def); AssignedInputsPassive = def.p->Controls; AssignedInputsAggressive = def.p->Controls; }
            else if (IsAllocated()) { g_startup.GetDef("FABLE_PC_CONTROL_SCHEME_GDD_ONE_MODE", &def); AssignedInputsPassive = def.p->Controls; AssignedInputsAggressive = def.p->Controls; }
        }
        ApplyValues();
    }
};

static int fail() { std::printf("004085f0_TEST FAIL\n"); return 1; }

int main()
{
    int x = 1;
    g_lcik.Controls.push_back(1); g_two.Controls.push_back(2); g_two.Controls.push_back(22);
    g_agg.Controls.push_back(3); g_one.Controls.push_back(4);
    { Profile p; g_bControlSchemeLeftClickIsKing = true; g_main = &x; g_log = "";
      p.ResetAssignedInputs();
      if (p.AssignedInputsPassive != g_lcik.Controls || p.AssignedInputsAggressive != g_lcik.Controls) return fail();
      if (g_log != "G" || p.applied != 1 || p.liveRefsAtApply != 0 || !g_lcik.deleted) return fail(); }
    { Profile p; g_bControlSchemeLeftClickIsKing = false; g_bAggressiveModeAvailable = 1; g_main = 0; g_start = &x; g_log = "";
      p.ResetAssignedInputs();
      if (p.AssignedInputsPassive != g_two.Controls || p.AssignedInputsAggressive != g_agg.Controls) return fail();
      if (g_log != "SS" || p.liveRefsAtApply != 0 || !g_two.deleted || !g_agg.deleted) return fail(); }
    { Profile p; g_bAggressiveModeAvailable = 0; g_main = &x; g_log = "";
      p.ResetAssignedInputs();
      if (p.AssignedInputsPassive != g_one.Controls || p.AssignedInputsAggressive != g_one.Controls || g_log != "G") return fail(); }
    { Profile p; p.AssignedInputsPassive.push_back(9); g_main = 0; g_start = 0; g_log = "";
      p.ResetAssignedInputs();
      if (g_log != "" || p.applied != 1 || p.AssignedInputsPassive.size() != 1 || p.AssignedInputsPassive[0] != 9) return fail(); }
    std::printf("004085f0_TEST PASS\n");
    return 0;
}