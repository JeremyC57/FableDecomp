#include <cstdio>
#include <cstring>
// Host model of CGameScriptInterface::AddCreatureScriptedMode: a valid thing with a mode manager (kind 0x31) gets a
// new scripted mode built from the thing and the script name.
struct Thing { unsigned int mask[8]; Thing() { for (int i = 0; i < 8; ++i) mask[i] = 0; } unsigned int HasTC(long k) const { return mask[k >> 5] & (1u << (k & 31)); } };
struct Mode { Thing* owner; const char* script; };
struct ModeManager { Mode* modes[4]; int count; ModeManager() : count(0) {} bool AddMode(Mode* m) { modes[count++] = m; return true; } };
struct ScriptThing { Thing* t; bool valid; bool IsValid() const { return valid; } Thing* GetThing() const { return t; } };
static Mode g_pool[4]; static int g_used = 0;
static ModeManager* g_manager = 0;
static ModeManager* GetModeManager(Thing*) { return g_manager; }
static void AddCreatureScriptedMode(const ScriptThing& thing, const char* name)
{
    if (!thing.IsValid()) return;
    Thing* t = thing.GetThing();
    if (!t->HasTC(0x31)) return;
    ModeManager* modes = GetModeManager(t);
    Mode* m = &g_pool[g_used++]; m->owner = thing.GetThing(); m->script = name;
    modes->AddMode(m);
}
int main()
{
    int fail = 0;
    Thing t; ModeManager mm; g_manager = &mm;
    ScriptThing s; s.t = &t; s.valid = false;
    AddCreatureScriptedMode(s, "ScriptA");
    s.valid = true;
    AddCreatureScriptedMode(s, "ScriptA");          // no mode manager bit
    if (mm.count != 0) fail = 1;
    t.mask[1] |= 0x20000u;                           // kind 0x31
    AddCreatureScriptedMode(s, "ScriptB");
    if (mm.count != 1 || mm.modes[0]->owner != &t || std::strcmp(mm.modes[0]->script, "ScriptB") != 0) fail = 1;
    if (fail) { std::printf("008a1a10_TEST FAIL\n"); return 1; }
    std::printf("008a1a10_TEST PASS\n");
    return 0;
}
