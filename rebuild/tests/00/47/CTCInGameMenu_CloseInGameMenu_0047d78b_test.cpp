// Behaviour test for CTCInGameMenu::CloseInGameMenu @ 0x0047D78B (volume restore, component teardown, inventory event)
#include <cstdio>

struct Profile { bool UseNew; float Sound, Dialogue, SoundNew, DialogueNew;
    float GetSoundVolume() const { return UseNew ? SoundNew : Sound; }
    float GetDialogueVolume() const { return UseNew ? DialogueNew : Dialogue; } };
struct Comp { static int Deleted; ~Comp() { ++Deleted; } };
int Comp::Deleted = 0;
struct Menu { bool Open; Comp* Components[4]; unsigned long Count; double Start, Paused; bool OpenInventory; void* DlgA; void* DlgB; };
static float g_vol[3]; static long g_cat[3]; static int g_volCalls; static long g_eventType = -1, g_eventData;
static void SetVolume(long c, float v) { g_cat[g_volCalls] = c; g_vol[g_volCalls] = v; ++g_volCalls; }
static void Close(Menu* m, const Profile& p, bool hasState, double now, bool heroAlive)
{
    if (hasState) { SetVolume(0x10, p.GetSoundVolume()); SetVolume(0x40, p.GetDialogueVolume()); SetVolume(0x80, p.GetSoundVolume()); }
    m->Paused += now - m->Start;
    for (unsigned long i = 0; i < m->Count; ++i) delete m->Components[i];
    m->Count = 0; m->DlgA = 0; m->DlgB = 0; m->Open = false;
    if (m->OpenInventory) { if (heroAlive) { g_eventType = 0x1A; g_eventData = 4; } m->OpenInventory = false; }
}
int main()
{
    Profile p; p.UseNew = true; p.Sound = 0.1f; p.Dialogue = 0.2f; p.SoundNew = 0.7f; p.DialogueNew = 0.8f;
    Menu m; m.Open = true; m.Count = 2; m.Components[0] = new Comp; m.Components[1] = new Comp; m.Start = 10.0; m.Paused = 1.0;
    m.OpenInventory = true; m.DlgA = &m; m.DlgB = &m;
    Close(&m, p, true, 15.5, true);
    if (g_volCalls != 3 || g_cat[1] != 0x40 || g_vol[0] != 0.7f || g_vol[1] != 0.8f || g_vol[2] != 0.7f || Comp::Deleted != 2 ||
        m.Count != 0 || m.Paused != 6.5 || m.Open || m.DlgA || m.OpenInventory || g_eventType != 0x1A || g_eventData != 4) { std::printf("0047d78b_TEST FAIL\n"); return 1; }
    p.UseNew = false; g_volCalls = 0; g_eventType = -1; m.OpenInventory = true;
    Close(&m, p, true, 20.0, false);
    if (g_vol[0] != 0.1f || g_vol[1] != 0.2f || g_eventType != -1 || m.OpenInventory) { std::printf("0047d78b_TEST FAIL\n"); return 1; }
    std::printf("0047d78b_TEST PASS\n");
    return 0;
}