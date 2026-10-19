// behaviour copy of NCreatureMode::CModeBlock::OnActivate: base activation, PCreature, suspend in state 1, START_BLOCKING sound, FrameUpdate
#include <cstdio>
#include <cstring>
static int g_seq[8]; static int g_n = 0;
struct CSoundPair { long First, Second; };
struct CSoundPlayParams {
    float Volume, Pitch, Delay; long Loop, Priority, Pan, Flags, Handle;
    CSoundPlayParams() { Volume = 1.0f; Pitch = 1.0f; Delay = 0.0f; Loop = 1; Pan = 0; Flags = 0; Handle = -1; Priority = 1; }
};
static const char* g_soundName = 0; static long g_played = 0; static long g_handle = 0;
struct CTCSoundPlayer { void PlaySound(const CSoundPair& s, const CSoundPlayParams& p, long, long, long) { g_played = s.First; g_handle = p.Handle; g_seq[g_n++] = 3; } };
struct CThingCreatureBase {
    unsigned char m_state; bool hasSound; CTCSoundPlayer player; bool suspended;
    CSoundPair GetSoundPair(const char* name) { g_soundName = name; CSoundPair p = { 42, 0 }; return p; }
    void SetSuspendAnimation(bool s, long) { suspended = s; g_seq[g_n++] = 2; }
    bool GetInterface(CTCSoundPlayer** out, long type) { if (type == 0x10 && hasSound) { *out = &player; return true; } return false; }
};
struct CModeBlock {
    CThingCreatureBase* ParentThing; CThingCreatureBase* PCreature;
    void BaseOnActivate() { g_seq[g_n++] = 1; }
    void FrameUpdate() { g_seq[g_n++] = 4; }
    void OnActivate()
    {
        BaseOnActivate();
        PCreature = ParentThing;
        if (PCreature->m_state == 1)
            PCreature->SetSuspendAnimation(true, 0);
        CTCSoundPlayer* player;
        if (ParentThing->GetInterface(&player, 0x10))
            player->PlaySound(PCreature->GetSoundPair("START_BLOCKING"), CSoundPlayParams(), 1, 0, 0);
        FrameUpdate();
    }
};
int main()
{
    CThingCreatureBase c; c.m_state = 1; c.hasSound = true; c.suspended = false;
    CModeBlock m; m.ParentThing = &c; m.PCreature = 0;
    m.OnActivate();
    if (m.PCreature != &c || !c.suspended || g_n != 4 || g_seq[0] != 1 || g_seq[1] != 2 || g_seq[2] != 3 || g_seq[3] != 4
        || std::strcmp(g_soundName, "START_BLOCKING") != 0 || g_played != 42 || g_handle != -1)
    { std::printf("008d8ce0_TEST FAIL\n"); return 1; }
    g_n = 0; c.m_state = 0; c.hasSound = false; m.OnActivate();
    if (g_n != 2 || g_seq[1] != 4) { std::printf("008d8ce0_TEST FAIL\n"); return 1; }
    std::printf("008d8ce0_TEST PASS\n");
    return 0;
}
