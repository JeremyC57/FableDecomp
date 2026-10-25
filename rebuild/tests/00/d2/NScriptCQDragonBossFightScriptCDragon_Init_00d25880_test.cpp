// Behaviour test for 00d25880: CDragon::Init -- the fight is set to the high-health state, then the three dragon thresholds come from the script def
#include <cstdio>
struct ScriptDef { float medium, low, veryLow; };
static ScriptDef g_def = { 0.75f, 0.5f, 0.25f };
static ScriptDef* g_pDef = &g_def;
struct Fight { int state; int calls; void SetDragonState(int s) { state = s; ++calls; } };
struct Dragon {
    Fight* parent; void* master; float medium, low, veryLow;
    void Init() { parent->SetDragonState(0); medium = g_pDef->medium; low = g_pDef->low; veryLow = g_pDef->veryLow; }
};
int main()
{
    Fight f; f.state = 3; f.calls = 0;
    Dragon d; d.parent = &f; d.master = 0; d.medium = d.low = d.veryLow = -1.0f;
    d.Init();
    int ok = f.state == 0 && f.calls == 1 && d.medium == 0.75f && d.low == 0.5f && d.veryLow == 0.25f && d.master == 0;
    std::printf(ok ? "00d25880_TEST PASS\n" : "00d25880_TEST FAIL\n");
    return ok ? 0 : 1;
}
