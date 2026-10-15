// Behaviour test for 0x00557000: Refresh clears, rebuilds from the profile inputs, relayouts, then (with a sound system) refreshes categories 0x10, 0x80, 0x40.
#include <cstdio>
static int g_log[16]; static int g_n;
struct Inputs { int id; };
struct Profile { Inputs in; static Profile& Get() { static Profile p = {{42}}; return p; } const Inputs& GetAssignedInputsPassive() { return in; } };
struct Sound { void RefreshCategory(int c, int, int) { g_log[g_n++] = c; } };
static Sound* g_pSoundSystem;
struct List { int rebuiltFrom; float off, time, warn; bool cleared;
  void ClearList() { cleared = true; g_log[g_n++] = 1; } void RefreshScriptThings(const Inputs& i) { rebuiltFrom = i.id; g_log[g_n++] = 2; }
  void UpdateVisibleList(float o, float t) { off = o; time = t; g_log[g_n++] = 3; } void UpdateUndefinedWarning(float t) { warn = t; g_log[g_n++] = 4; }
  void Refresh() { ClearList(); RefreshScriptThings(Profile::Get().GetAssignedInputsPassive()); UpdateVisibleList(0.0f, 0.2f); UpdateUndefinedWarning(0.2f);
    if (g_pSoundSystem) { g_pSoundSystem->RefreshCategory(0x10, 0, 0); g_pSoundSystem->RefreshCategory(0x80, 0, 0); g_pSoundSystem->RefreshCategory(0x40, 0, 0); } } };
int main()
{
    Sound s; List l = {0, 1.0f, 1.0f, 1.0f, false};
    g_pSoundSystem = 0; g_n = 0; l.Refresh(); bool ok = g_n == 4 && l.rebuiltFrom == 42 && l.off == 0.0f && l.time == 0.2f && l.warn == 0.2f;
    g_pSoundSystem = &s; g_n = 0; l.Refresh(); ok = ok && g_n == 7 && g_log[4] == 0x10 && g_log[5] == 0x80 && g_log[6] == 0x40;
    if (!ok) { std::printf("00557000_TEST FAIL\n"); return 1; }
    std::printf("00557000_TEST PASS\n");
    return 0;
}