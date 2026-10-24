// CSubtitleRenderer::IsTextReadyForClickPast() const -- the shown text may be clicked past: the cutscene allows skipping (or a forced/click display flag 0x48 is set), the text is complete (flag 2) and has been up for more than 0.2 s
// Retail: Fable.exe (TLC, VC7.1) @ 0x0048E072, 48 bytes, plain `ret`.
// Callees: CSubtitleRenderer::IsCutsceneSkippable @ 0x0048DE44 (landed; same TU). Constant: 0.2f @ 0x012305A0.
// Shape: `if (!IsCutsceneSkippable() && !(DisplayFlags & 0x48)) return false; return (DisplayFlags & 2) &&
// TextTimer > 0.2f;` in the subtitle renderer's size TU. Retail keeps `this` in ecx across the call because the
// callee is compiled EARLIER in the same TU and VC7.1 knows it leaves ecx alone (its body is reproduced above
// under `auto_inline(off)`); the false exit after `test al,al` returns the callee's al as is, and the `&&`
// result is the full-register 0/1 (`xor eax,eax ; inc eax`) of a size TU.
// Name: the catalog label "CSubtitleRenderer_EvaluateCutsceneSkippability" and the landed caller's
// "IsTextFullyShown?" are guesses; Ego_r.pdb's CSubtitleRenderer has `?IsTextReadyForClickPast@CSubtitleRenderer@@QBE_NXZ`,
// the only const bool no-argument member that fits a click-past test (IsWaitingForClickPast 0x0048DB46 and
// IsInInteractiveState 0x00490A22 are landed; IsCameraReadyToChange/IsThirdButtonActive read other state).
// Layout: the local retail model of the landed 0x0048DE44 (DisplayFlags +0x7C, TextTimer +0x8C; the engine
// header is the PDB view with 16-byte vectors, 0x10 lower).
// Leaf: IsTextReadyForClickPast
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
    bool IsTextReadyForClickPast() const;
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

#pragma auto_inline(off)
bool CSubtitleRenderer::IsCutsceneSkippable() const      // 0x0048DE44 (landed body, same TU)
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
#pragma auto_inline(on)

bool CSubtitleRenderer::IsTextReadyForClickPast() const
{
    if (!IsCutsceneSkippable() && !(DisplayFlags & 0x48))
        return false;
    return (DisplayFlags & 2) && TextTimer > 0.2f;
}