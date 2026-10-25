// Behaviour test for CWorld::EASkipCutScene (0x00629F50): only a skippable cutscene is skipped: button sound
// played once with the 0x8010 fader init, the skip message posted, subtitles cleared and the flags flipped.
#include <cstdio>
struct CCharString { const char* Text; };
struct C3DVector { float X, Y, Z; C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {} };
struct CThing; struct CGameEvent; struct CASoundBank { int id; };
struct CSoundInit {
    float Volume, Pitch, Pan; long NoPlays, PlaybackType; unsigned long GainFaderBits, PitchFaderBits; long OverrideID;
    CSoundInit() : Volume(1.0f), Pitch(1.0f), Pan(0.0f), NoPlays(1), PitchFaderBits(0), OverrideID(-1) { PlaybackType = 1; GainFaderBits = 0; }
};
static int g_plays; static unsigned long g_bits; static const char* g_sound; static CASoundBank* g_bankSeen; static int g_loop, g_att;
struct CSoundPlayer {
    unsigned long PlaySoundEvent(CASoundBank& b, const CCharString& n, bool loop, CSoundInit& init, bool att)
    { ++g_plays; g_bits = init.GainFaderBits; g_sound = n.Text; g_bankSeen = &b; g_loop = loop; g_att = att; return 1; }
};
static CSoundPlayer g_player;
struct CSoundSystem { CSoundPlayer* GetSoundPlayer() { return &g_player; } };
static CSoundSystem g_sys; static CSoundSystem* g_pSoundSystem = &g_sys;
struct CPlayerGuiDef { CCharString TextBoxButtonBSoundCriteria; };
static CPlayerGuiDef g_def = { { "SOUND_BUTTON_A" } };
static CPlayerGuiDef* PeekGuiDef() { return &g_def; }
static int g_clears;
struct CSubtitleRenderer {
    bool InCutscene, CutsceneSkipped, CutsceneSkippable;
    bool IsCutsceneSkippable() const { return CutsceneSkippable; }
    void Clear(bool all) { if (!all) ++g_clears; }
};
struct CPlayerGui { CSubtitleRenderer SubtitleRenderer; };
static CPlayerGui g_gui; static CPlayerGui* g_pPlayerGui = &g_gui;
static unsigned long g_framesPerSecond = 30;
static long g_msgType; static unsigned long g_msgDelay; static int g_msgs;
struct CMessageEventManager {
    void* AddMessage(long t, unsigned long d, const C3DVector& p, CThing*, CThing*, float, float, long)
    { ++g_msgs; g_msgType = t; g_msgDelay = d; return p.X == 0.0f && p.Y == 0.0f && p.Z == 0.0f ? this : 0; }
};
static CASoundBank g_bank;
struct CWorld {
    CMessageEventManager* PMessageEventManager;
    CASoundBank& GetSampleBank() const { return g_bank; }
    void EASkipCutScene(const CGameEvent*)
    {
        CSubtitleRenderer& subtitles = g_pPlayerGui->SubtitleRenderer;
        if (subtitles.InCutscene && subtitles.IsCutsceneSkippable()) {
            CPlayerGuiDef* def = PeekGuiDef();
            CSoundInit init;
            init.GainFaderBits = 0x8010;
            g_pSoundSystem->GetSoundPlayer()->PlaySoundEvent(GetSampleBank(), def->TextBoxButtonBSoundCriteria, true, init, false);
            PMessageEventManager->AddMessage(0x32, g_framesPerSecond, C3DVector(0.0f, 0.0f, 0.0f), 0, 0, 0.0f, 0.0f, 0);
            subtitles.Clear(false);
            subtitles.CutsceneSkipped = true;
            subtitles.CutsceneSkippable = false;
        }
    }
};
int main()
{
    CMessageEventManager m; CWorld w; w.PMessageEventManager = &m;
    CSubtitleRenderer& s = g_gui.SubtitleRenderer;
    s.InCutscene = false; s.CutsceneSkipped = false; s.CutsceneSkippable = true;
    w.EASkipCutScene(0);
    bool ok = g_plays == 0 && g_msgs == 0 && !s.CutsceneSkipped;
    s.InCutscene = true; s.CutsceneSkippable = false;
    w.EASkipCutScene(0);
    ok = ok && g_plays == 0 && g_msgs == 0;
    s.CutsceneSkippable = true;
    w.EASkipCutScene(0);
    ok = ok && g_plays == 1 && g_bits == 0x8010 && g_sound == g_def.TextBoxButtonBSoundCriteria.Text && g_bankSeen == &g_bank
        && g_loop == 1 && g_att == 0 && g_msgs == 1 && g_msgType == 0x32 && g_msgDelay == 30 && g_clears == 1
        && s.CutsceneSkipped && !s.CutsceneSkippable;
    if (!ok) { std::printf("00629f50_TEST FAIL\n"); return 1; }
    std::printf("00629f50_TEST PASS\n");
    return 0;
}