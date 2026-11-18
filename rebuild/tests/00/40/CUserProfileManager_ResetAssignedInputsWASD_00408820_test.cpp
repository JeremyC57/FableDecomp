// Behaviour test for CUserProfileManager::ResetAssignedInputsWASD (0x00408820): the aggressive-mode early path (ResetAssignedInputs,
// no ApplyValues of its own), the caps-lock scheme choice, the game / startup manager choice, the release of the def before
// ApplyValues and the two vector copies.
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
static Def g_wasd, g_nocaps;
static std::string g_log;
struct Mgr {
    const char* tag;
    bool GetDef(const std::string& name, DefPtr* out) const {
        Def* d = 0;
        if (name == "FABLE_PC_CONTROL_SCHEME_GDD_WASD") d = &g_wasd;
        else if (name == "FABLE_PC_CONTROL_SCHEME_GDD_WASD_NO_CAPSLOCK") d = &g_nocaps;
        g_log += tag;
        if (!d) return false;
        if (out->p != d) { if (out->p && --out->p->refs == 0) out->p->deleted = 1; out->p = d; ++d->refs; }
        return true;
    }
};
static Mgr g_game = { "G" }, g_startup = { "S" };
static unsigned char g_bAggressiveModeAvailable, g_KeyRedefinerFlag;
static void* g_main; static void* g_start;
static bool IsAllocated() { return g_start != 0; }

struct Profile {
    std::vector<int> AssignedInputsPassive, AssignedInputsAggressive;
    int applied; int reset; int liveRefsAtApply;
    Profile() : applied(0), reset(0), liveRefsAtApply(-1) {}
    void ApplyValues() { ++applied; liveRefsAtApply = g_wasd.refs + g_nocaps.refs; }
    void ResetAssignedInputs() { ++reset; ApplyValues(); }
    void ResetAssignedInputsWASD()
    {
        if (g_bAggressiveModeAvailable) {
            ResetAssignedInputs();
            return;
        }
        {
            DefPtr pdef;
            std::string controlDefName(g_KeyRedefinerFlag ? "FABLE_PC_CONTROL_SCHEME_GDD_WASD_NO_CAPSLOCK" : "FABLE_PC_CONTROL_SCHEME_GDD_WASD");
            if (g_main) {
                g_game.GetDef(controlDefName, &pdef);
                AssignedInputsPassive = pdef.p->Controls;
                AssignedInputsAggressive = pdef.p->Controls;
            } else if (IsAllocated()) {
                g_startup.GetDef(controlDefName, &pdef);
                AssignedInputsPassive = pdef.p->Controls;
                AssignedInputsAggressive = pdef.p->Controls;
            }
        }
        ApplyValues();
    }
};

static int fail(int n) { std::printf("00408820_TEST FAIL %d\n", n); return 1; }

int main()
{
    int x = 1;
    g_wasd.Controls.push_back(1); g_wasd.Controls.push_back(11); g_nocaps.Controls.push_back(2);
    { Profile p; g_bAggressiveModeAvailable = 1; g_main = &x; g_log = "";
      p.ResetAssignedInputsWASD();
      if (p.reset != 1 || p.applied != 1 || g_log != "" || !p.AssignedInputsPassive.empty()) return fail(1); }
    { Profile p; g_bAggressiveModeAvailable = 0; g_KeyRedefinerFlag = 0; g_main = &x; g_log = "";
      p.ResetAssignedInputsWASD();
      if (p.AssignedInputsPassive != g_wasd.Controls || p.AssignedInputsAggressive != g_wasd.Controls) return fail(2);
      if (g_log != "G" || p.reset != 0 || p.applied != 1 || p.liveRefsAtApply != 0 || !g_wasd.deleted) return fail(3); }
    { Profile p; g_KeyRedefinerFlag = 1; g_main = 0; g_start = &x; g_log = "";
      p.ResetAssignedInputsWASD();
      if (p.AssignedInputsPassive != g_nocaps.Controls || p.AssignedInputsAggressive != g_nocaps.Controls) return fail(4);
      if (g_log != "S" || p.applied != 1 || p.liveRefsAtApply != 0 || !g_nocaps.deleted) return fail(5); }
    { Profile p; p.AssignedInputsPassive.push_back(9); g_main = 0; g_start = 0; g_log = "";
      p.ResetAssignedInputsWASD();
      if (g_log != "" || p.applied != 1 || p.AssignedInputsPassive.size() != 1 || p.AssignedInputsPassive[0] != 9) return fail(6); }
    std::printf("00408820_TEST PASS\n");
    return 0;
}