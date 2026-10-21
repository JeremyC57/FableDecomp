// Grade: DIFFER(370v357) -- block layout only: retail places the shared return-false epilogue right after the TextCharactersShowing block (other exits jump back to it) and cross-jumps the A arm's `test ah,2` into the B arm's je; ours keeps the epilogue last
// CSubtitleRenderer::ClickPast() -- the player pressed "continue": reveal the whole line, or move to the next tagged text, or clear the subtitle (with a click sound)
// Leaf: ClickPast
// Retail 0x00493281, 357 bytes, ebp frame, `ret` (size-optimised TU: `#pragma optimize("s", on)`).
// Callees: CSubtitleRenderer::EvaluateCutsceneSkippability 0x0048E072, PeekGuiDef 0x00438570 (__fastcall CPlayerGuiDef*),
// CSoundInit::CSoundInit 0x0041C930, slot 9 (+0x24) of the game-state singleton 0x013B8394 (the sound manager) and that
// manager's slot 4 (+0x10, play a 2D sound), CWideString::GetLength 0x0099B220, CTBaseSingleton<CUserProfileManager>::Get
// 0x0040D2A0, CWideString::operator!=(const wchar_t*) 0x0099B150 (L"" 0x0122D70C), GetTextFromTextTag 0x0048F6BD,
// ProcessEmbeddedGraphicTags 0x00492381, ProcessEmbeddedHighlightTags 0x00491AD2, Clear(bool) 0x00492BAB.
// Globals: the game state 0x013B8394 and the engine component 0x013B86A0 (sample bank +0x10).
// Layout: the retail model of the landed constructor 0x0049166E (TextCharactersShowing +0x24, TextGroupTag +0x30, Text +0x5C,
// DisplayFlags +0x7C, AllCharactersDisplayed +0xDD, InCutscene +0xDE, DisplayingText +0xE5); the GUI def's click sound is at +0x5E8.
// Shape: the click sound is an inline helper expanded on both arms; the profile flag is an inline `a ? b : c` byte accessor.
#pragma optimize("s", on)
struct CWideStringData;
struct CWideString {
    CWideStringData* PStringData;
    long GetLength() const;                                   // 0x0099B220
    bool operator!=(const unsigned short* text) const;        // 0x0099B150
};
struct CSoundInit {
    float Volume, Pitch;
    long  Delay, LoopCount, Priority, Flags, Pan, Id;
    CSoundInit();                                             // 0x0041C930
};
struct CSoundBank;
struct CSoundName;
struct CSoundManager {
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void Play2DSound(CSoundBank* bank, const CSoundName& name, long a, const CSoundInit& init, long b);   // +0x10
};
struct CGameState {
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4(); virtual void v5();
    virtual void v6(); virtual void v7(); virtual void v8();
    virtual CSoundManager* GetSoundManager();                 // +0x24
};
extern CGameState* g_pGameState;                              // 0x013B8394
struct CMainGameComponent {
    unsigned char _pad00[0x10];
    CSoundBank*   PSampleBank;                                // +0x10
};
extern CMainGameComponent* g_pMainGameComponent;              // 0x013B86A0
struct CPlayerGuiDef {
    unsigned char _pad000[0x5e8];
    unsigned char ClickSound[4];                              // +0x5e8 (CSoundName)
};
CPlayerGuiDef* __fastcall PeekGuiDef();                       // 0x00438570
struct CUserProfileManager {
    unsigned char _pad00[0x0c];
    bool          UsingAltProfile;                            // +0x0c
    bool          ProfileFlag;                                // +0x0d
    unsigned char _pad0e[0xcd - 0x0e];
    bool          AltProfileFlag;                             // +0xcd
    __forceinline bool GetFlag() const { return UsingAltProfile ? AltProfileFlag : ProfileFlag; }
};
template <class T> struct CTBaseSingleton {
    static T& __fastcall Get();                               // 0x0040D2A0
};

struct CSubtitleRenderer {
    virtual void v00();
    unsigned char _pad04[0x24 - 0x04];
    long          TextCharactersShowing;                      // +0x24
    unsigned char _pad28[0x30 - 0x28];
    CWideString   TextGroupTag;                               // +0x30
    unsigned char _pad34[0x5c - 0x34];
    CWideString   Text;                                       // +0x5c
    unsigned char _pad60[0x7c - 0x60];
    unsigned long DisplayFlags;                               // +0x7c
    unsigned char _pad80[0xdc - 0x80];
    bool          PausedGame;                                 // +0xdc
    bool          AllCharactersDisplayed;                     // +0xdd
    bool          InCutscene;                                 // +0xde
    bool          CutsceneSkipped;                            // +0xdf
    bool          CutsceneSkippable;                          // +0xe0
    bool          CutsceneSkippableWhilePaused;               // +0xe1
    bool          CutsceneActionMode;                         // +0xe2
    bool          Button1TextOverridden;                      // +0xe3
    bool          Button2TextOverridden;                      // +0xe4
    bool          DisplayingText;                             // +0xe5
    bool EvaluateCutsceneSkippability();                      // 0x0048E072
    bool GetTextFromTextTag(const CWideString& tag, CWideString& text);   // 0x0048F6BD
    void ProcessEmbeddedGraphicTags();                        // 0x00492381
    void ProcessEmbeddedHighlightTags();                      // 0x00491AD2
    void Clear(bool all);                                     // 0x00492BAB
    __forceinline void PlayClickSound()
    {
        if ((DisplayFlags & 2) && g_pGameState) {
            CPlayerGuiDef* def = PeekGuiDef();
            CSoundInit init;
            init.Flags = 0x8010;
            g_pGameState->GetSoundManager()->Play2DSound(g_pMainGameComponent->PSampleBank,
                                                         *(const CSoundName*)def->ClickSound, 1, init, 0);
        }
    }
    bool ClickPast();
};

bool CSubtitleRenderer::ClickPast()
{
    if (DisplayingText) {
        if (InCutscene && !(DisplayFlags & 0x48))
            return false;
        if (AllCharactersDisplayed) {
            if (!EvaluateCutsceneSkippability())
                return false;
            PlayClickSound();
            if (!(DisplayFlags & 8) && !(DisplayFlags & 0x200))
                goto clear;
            goto next;
        }
        TextCharactersShowing = Text.GetLength();
        return false;
    }
    if (CTBaseSingleton<CUserProfileManager>::Get().GetFlag())
        return false;
    if (InCutscene)
        return false;
    PlayClickSound();
    if (!(DisplayFlags & 0x200))
        goto clear;
next:
    if (TextGroupTag != (const unsigned short*)L"" && GetTextFromTextTag(TextGroupTag, Text)) {
        ProcessEmbeddedGraphicTags();
        ProcessEmbeddedHighlightTags();
        return false;
    }
clear:
    Clear(false);
    return true;
}
