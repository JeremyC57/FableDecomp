// CSubtitleRenderer::Update(bool paused) -- per-frame subtitle tick: advance the display timer, clear the text once its display time has run out (click-past text only when the profile does not wait for a click, or in a cutscene) and reveal more characters
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) @ 0x004930D7, 197 bytes, `ret 4`. Ego_r.pdb `?Update@CSubtitleRenderer@@QAEX_N@Z`.
// Leaf: Update
// Callees: CTBaseSingleton<CUserProfileManager>::Get @ 0x0040D2A0 (`__fastcall`), CSubtitleRenderer::Clear(bool) @ 0x00492BAB,
//   PeekGuiDef @ 0x00438570 (`__fastcall`); globals: the frame rate @ 0x01375550 (long), float pool 1.0f @ 0x0122DED8 and
//   0.0f @ 0x0122DEDC.
// Shape: size TU without the frame pointer (unlike its neighbours); one zero register (bl) for the flag compares and the Clear
//   argument; the timer step is `1.0f / fps` added in place; `MaxDisplayTime != 0.0f` is one fucompp shared by both
//   display-mode arms (the click-past bit picks the arm after the compare), `TextTimer > MaxDisplayTime` is evaluated in each
//   arm; the profile flag is an inline accessor choosing between the active profile's and the default byte (as the landed
//   CGameScriptInterface::MsgIsTutorialClickedPast 0x008943C0); the reveal adds the GUI def's characters-per-frame.
// Retail layout (the landed CSubtitleRenderer rows): TextCharactersShowing +0x24, DisplayFlags +0x7C, MaxDisplayTime +0x88,
//   TextTimer +0x8C, InCutscene +0xDE, DisplayingText +0xE5, UpdateWhilePaused +0xE6, UpdateTimer +0xE7; CPlayerGuiDef
//   characters per frame retail +0x90C.
extern long g_FramesPerSecond;                          // 0x01375550

class CPlayerGuiDef { public:
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