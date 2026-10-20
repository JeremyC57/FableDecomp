// Behaviour test for CSubtitleRenderer::Update (0x004930D7): timer step, timed clear rules (click-past mode honours the profile unless in a cutscene) and the per-frame reveal.
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif
extern long g_FramesPerSecond;                          // 0x01375550

struct CPlayerGuiDef {
    unsigned char _pad[0x90c];
    long          TextCharactersPerFrame;               // +0x90c (retail)
};
extern CPlayerGuiDef* __fastcall PeekGuiDef();          // 0x00438570

class CUserProfileManager {
public:
    unsigned char _pad00[0x0c];
    bool          m_hasActiveProfile;                   // +0x0c
    bool          m_defaultWaitForClickPast;            // +0x0d
    unsigned char _pad0e[0xcd - 0x0e];
    bool          m_profileWaitForClickPast;            // +0xcd
    bool IsWaitForClickPast() const { return m_hasActiveProfile ? m_profileWaitForClickPast : m_defaultWaitForClickPast; }
};
template <class T> struct CTBaseSingleton {
    static T& __fastcall Get();                         // 0x0040D2A0
};
template <> CUserProfileManager& __fastcall CTBaseSingleton<CUserProfileManager>::Get();

class CSubtitleRenderer {
public:
    void*         __vftable;                            // +0x00
    unsigned char _pad04[0x24 - 0x04];
    long          TextCharactersShowing;                // +0x24
    unsigned char _pad28[0x7c - 0x28];
    unsigned long DisplayFlags;                         // +0x7c
    unsigned char _pad80[0x88 - 0x80];
    float         MaxDisplayTime;                       // +0x88
    float         TextTimer;                            // +0x8c
    unsigned char _pad90[0xde - 0x90];
    bool          InCutscene;                           // +0xde
    unsigned char _padDF[0xe5 - 0xdf];
    bool          DisplayingText;                       // +0xe5
    bool          UpdateWhilePaused;                    // +0xe6
    bool          UpdateTimer;                          // +0xe7
    void Clear(bool flag);                              // 0x00492BAB
    void Update(bool paused);
};

void CSubtitleRenderer::Update(bool paused)
{
    if (!DisplayingText)
        return;
    if (!UpdateWhilePaused && paused)
        return;
    if (UpdateTimer)
        TextTimer += 1.0f / g_FramesPerSecond;
    if (!(DisplayFlags & 0xc)) {
        if (DisplayFlags & 2) {
            if (MaxDisplayTime != 0.0f && TextTimer > MaxDisplayTime
                && (!CTBaseSingleton<CUserProfileManager>::Get().IsWaitForClickPast() || InCutscene))
                Clear(false);
        } else if (MaxDisplayTime != 0.0f && TextTimer > MaxDisplayTime)
            Clear(false);
    }
    TextCharactersShowing += PeekGuiDef()->TextCharactersPerFrame;
}
long g_FramesPerSecond = 4;
static CPlayerGuiDef g_def;
CPlayerGuiDef* __fastcall PeekGuiDef() { return &g_def; }
static CUserProfileManager g_pm;
template <> CUserProfileManager& __fastcall CTBaseSingleton<CUserProfileManager>::Get() { return g_pm; }
static int g_clears = 0;
void CSubtitleRenderer::Clear(bool flag) { if (!flag) ++g_clears; }
static bool Run(bool displaying, bool whilePaused, bool paused, bool timer, unsigned long flags, float maxTime, float t,
                bool hasProfile, bool wait, bool cutscene, int wantClears, float wantTimer, long wantShowing)
{
    static CSubtitleRenderer r;
    g_clears = 0; g_def.TextCharactersPerFrame = 3;
    g_pm.m_hasActiveProfile = hasProfile; g_pm.m_profileWaitForClickPast = wait; g_pm.m_defaultWaitForClickPast = !wait;
    r.DisplayingText = displaying; r.UpdateWhilePaused = whilePaused; r.UpdateTimer = timer; r.DisplayFlags = flags;
    r.MaxDisplayTime = maxTime; r.TextTimer = t; r.InCutscene = cutscene; r.TextCharactersShowing = 10;
    r.Update(paused);
    return g_clears == wantClears && r.TextTimer == wantTimer && r.TextCharactersShowing == wantShowing;
}
int main()
{
    bool ok = Run(false, true, false, true, 0, 1.0f, 2.0f, false, false, false, 0, 2.0f, 10)      /* not displaying */
           && Run(true, false, true, true, 0, 1.0f, 2.0f, false, false, false, 0, 2.0f, 10)       /* paused */
           && Run(true, true, true, true, 0, 1.0f, 2.0f, false, false, false, 1, 2.25f, 13)       /* timed out: clear */
           && Run(true, true, false, false, 0, 0.0f, 2.0f, false, false, false, 0, 2.0f, 13)      /* no max time */
           && Run(true, true, false, false, 0, 3.0f, 2.0f, false, false, false, 0, 2.0f, 13)      /* not yet */
           && Run(true, true, false, false, 4, 1.0f, 2.0f, false, false, false, 0, 2.0f, 13)      /* flag 4: never */
           && Run(true, true, false, false, 2, 1.0f, 2.0f, true, true, false, 0, 2.0f, 13)        /* click past, profile waits */
           && Run(true, true, false, false, 2, 1.0f, 2.0f, true, true, true, 1, 2.0f, 13)         /* ... but in a cutscene */
           && Run(true, true, false, false, 2, 1.0f, 2.0f, false, true, false, 1, 2.0f, 13);      /* default does not wait */
    if (!ok) { std::printf("004930d7_TEST FAIL\n"); return 1; }
    std::printf("004930d7_TEST PASS\n");
    return 0;
}