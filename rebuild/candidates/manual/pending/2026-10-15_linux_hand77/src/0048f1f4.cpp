// CSubtitleRenderer::Draw2DText(const C2DVector&, const CWideString&, CFontBank*, const CRGBColour&, bool, float, float, bool) -- draws text through the game UI on layer 0x43 (0x40 with display flag 0x800), one layer higher when asked
// Leaf: Draw2DText
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0048F1F4, 60 bytes. Callee: CGameUI::Draw2DText @ 0x00438010 (forwards to
// CDraw2DText::AddText through +0x238). Global: the game UI pointer @ 0x013B8790. The sixth argument is unused.
// Layout: engine/CSubtitleRenderer.h is the PDB view (Ego_r: 16-byte vectors, a 0x18-byte deque); retail has the VC6
// containers (0x28-byte deque, 12-byte vectors), so DisplayFlags sits at +0x7C (PDB +0x6C) and the cutscene bools at
// +0xDC.. (PDB +0xE4..). The header disagrees with the bytes, so the class is modelled locally with the PDB names.
// Shape: `push 43h ; pop eax` immediates and pass-through `push dword ptr [ebp+n]` with a kept frame pointer =
// #pragma optimize("y", off) + ("s", on); the layer is a long local adjusted with `dec eax`.
#pragma optimize("y", off)
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
    void Draw2DText(const C2DVector& pos, const CWideString& text, CFontBank* font, const CRGBColour& colour,
                    bool flag, float unused, float scale, bool raiseLayer);
};

enum EEngine2DLayer { ENGINE_2D_LAYER_SUBTITLE = 0x43 };

struct CGameUI {
    void Draw2DText(const C2DVector& pos, const CWideString& text, CFontBank* font, const CRGBColour& colour,
                    float scale, EEngine2DLayer layer, bool flag);   // 0x00438010
};
extern CGameUI* g_pGameUI;                         // 0x013B8790

void CSubtitleRenderer::Draw2DText(const C2DVector& pos, const CWideString& text, CFontBank* font, const CRGBColour& colour,
                                   bool flag, float unused, float scale, bool raiseLayer)
{
    long layer = 0x43;
    if (DisplayFlags & 0x800)
        layer = 0x40;
    if (raiseLayer)
        --layer;
    g_pGameUI->Draw2DText(pos, text, font, colour, scale, (EEngine2DLayer)layer, flag);
}
