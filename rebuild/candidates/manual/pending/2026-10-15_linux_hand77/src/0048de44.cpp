// CSubtitleRenderer::IsCutsceneSkippable() const -- false while the display engine is busy (+0x104); true outside cutscenes; inside one only when the cutscene is skippable, not yet skipped, no forced-wait text is up and the skip prompt has finished opening
// Leaf: IsCutsceneSkippable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0048DE44, 70 bytes. No callees. Global: the game-component pointer @
// 0x013B86A0 (its +0x24 word is the display engine, as the landed CViewBase::GetDisplayEngine getters read it).
// Layout: engine/CSubtitleRenderer.h is the PDB view (Ego_r: 16-byte vectors, a 0x18-byte deque); retail has the VC6
// containers (0x28-byte deque, 12-byte vectors), so DisplayFlags sits at +0x7C (PDB +0x6C) and the cutscene bools at
// +0xDC.. (PDB +0xE4..). The header disagrees with the bytes, so the class is modelled locally with the PDB names.
// Shape: one zero register (`xor eax,eax ; cmp [mem],al`) serves every compare = a size TU (#pragma optimize("s",
// on)); the in-cutscene checks are NESTED under `if (InCutscene)` so the not-in-cutscene path and the last test share
// the trailing `mov al,1` (flat early returns end in `sete al`); the early-outs share the `xor al,al` exit.
#pragma optimize("s", on)
class CWideString { public: void* PStringData; };

struct C2DVector { float X, Y; };
struct C2DBoxF { float TLX, TLY, BRX, BRY; };
struct CRGBColour { unsigned char B, G, R, A; };
class CFontBank;
class CGraphicDataBank;

struct CFadeInFadeOutBase {
    bool       Active;                             // +0x00
    float      FadeInTime;                         // +0x04
    float      FadeOutTime;                        // +0x08
    bool       Closing;                            // +0x0C
    bool       Opening;                            // +0x0D
    float      OpenTimer;                          // +0x10
    float      CloseTimer;                         // +0x14
    CRGBColour ToColour;                           // +0x18
};

struct CRetailVector { void* m_pFirst; void* m_pLast; void* m_pEnd; };   // VC6 vector: 12 bytes

class CSubtitleRenderer {
public:
    void*              __vftable;                  // +0x00
    C2DBoxF            VWindow;                    // +0x04
    C2DBoxF            VTelevisionWindow;          // +0x14
    long               TextCharactersShowing;      // +0x24
    bool               TextGroupHasEntriesRemaining; // +0x28
    long               TextGroupIndex;             // +0x2C
    CWideString        TextGroupTag;               // +0x30
    unsigned char      TextInfoQueue[0x28];        // +0x34 (VC6 deque<CSetTextInfo>)
    CWideString        Text;                       // +0x5C
    CWideString        NameText;                   // +0x60
    CWideString        Button1Text;                // +0x64
    CWideString        Button2Text;                // +0x68
    CWideString        Button3Text;                // +0x6C
    CWideString        Button1OverrideText;        // +0x70
    CWideString        Button2OverrideText;        // +0x74
    unsigned char      Button1Alpha;               // +0x78
    unsigned char      Button2Alpha;               // +0x79
    unsigned long      DisplayFlags;               // +0x7C
    CFontBank*         PFont;                      // +0x80
    CGraphicDataBank*  Graphics;                   // +0x84
    float              MaxDisplayTime;             // +0x88
    float              TextTimer;                  // +0x8C
    long               PlayerModeAdded;            // +0x90
    CRetailVector      PrimitiveHandles;           // +0x94
    CRetailVector      ButtonPrimitiveHandles;     // +0xA0
    CRetailVector      EmbeddedPrimitiveHandles;   // +0xAC
    CRetailVector      EmbeddedGraphics;           // +0xB8
    CRetailVector      EmbeddedHighlights;         // +0xC4
    CRetailVector      EmbeddedHighlightActionNames; // +0xD0
    bool               PausedGame;                 // +0xDC
    bool               AllCharactersDisplayed;     // +0xDD
    bool               InCutscene;                 // +0xDE
    bool               CutsceneSkipped;            // +0xDF
    bool               CutsceneSkippable;          // +0xE0
    bool               CutsceneSkippableWhilePaused; // +0xE1
    bool               CutsceneActionMode;         // +0xE2
    bool               Button1TextOverridden;      // +0xE3
    bool               Button2TextOverridden;      // +0xE4
    bool               DisplayingText;             // +0xE5
    bool               UpdateWhilePaused;          // +0xE6
    bool               UpdateTimer;                // +0xE7
    CFadeInFadeOutBase SkipCutsceneTimer;          // +0xE8
    CFadeInFadeOutBase ButtonPulseTimer;           // +0x104
    bool IsCutsceneSkippable() const;
};

struct CDisplayEngine_0048de44 {
    unsigned char _pad[0x104];
    long          m_busy;                          // +0x104
};
struct CGameComponent_0048de44 {
    unsigned char             _pad[0x24];
    CDisplayEngine_0048de44*  m_pDisplayEngine;    // +0x24
};
extern CGameComponent_0048de44* g_pGameComponent;   // 0x013B86A0

bool CSubtitleRenderer::IsCutsceneSkippable() const
{
    if (g_pGameComponent->m_pDisplayEngine->m_busy != 0)
        return false;
    if (InCutscene) {
        if (!CutsceneSkippable)
            return false;
        if (CutsceneSkipped)
            return false;
        if (DisplayFlags & 0x48)
            return false;
        if (!SkipCutsceneTimer.Active)
            return false;
        if (SkipCutsceneTimer.Opening)
            return false;
    }
    return true;
}
