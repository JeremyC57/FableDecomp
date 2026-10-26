// CSubtitleRenderer::GetBlackBarHeight() const -- half the gap between the virtual screen height and a 16:9 band of the virtual screen width: (VY(height) - VX(width) * 0.5625) * 0.5
// Leaf: GetBlackBarHeight
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0048DBC3, 54 bytes. Callees: GFAYToVY(float) @ 0x009E1E40 and
// GFAXToVX(float) @ 0x009E1E10 (float in a stack slot, callee-popped: __stdcall), the screen size doubles @ 0x013961F0
// (height) / 0x013961E8 (width) narrowed to float, the constants 0.5625f @ 0x01238110 and 0.5f @ 0x0122F59C.
// Layout: engine/CSubtitleRenderer.h is the PDB view (Ego_r: 16-byte vectors, a 0x18-byte deque); retail has the VC6
// containers (0x28-byte deque, 12-byte vectors), so DisplayFlags sits at +0x7C (PDB +0x6C) and the cutscene bools at
// +0xDC.. (PDB +0xE4..). The header disagrees with the bytes, so the class is modelled locally with the PDB names.
// Shape: frame pointer with `push ecx` for the named float local that holds the first call's result across the
// second call, `leave` = #pragma optimize("y", off) + ("s", on); `call ; fmul [0.5625]` is `VX(..) * 0.5625f`, `fsubr
// [local]` the subtraction from the saved height.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CWideString { public: void* PStringData; };

class C2DVector { public: float X, Y; };
class C2DBoxF { public: float TLX, TLY, BRX, BRY; };
class CRGBColour { public: unsigned char B, G, R, A; };
class CFontBank;
class CGraphicDataBank;

class CFadeInFadeOutBase { public:
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
    float GetBlackBarHeight() const;
};

float __stdcall GFAYToVY(float y);                 // 0x009E1E40
float __stdcall GFAXToVX(float x);                 // 0x009E1E10
extern double g_screenHeight;                      // 0x013961F0
extern double g_screenWidth;                       // 0x013961E8

float CSubtitleRenderer::GetBlackBarHeight() const
{
    float height = GFAYToVY((float)g_screenHeight);
    return (height - GFAXToVX((float)g_screenWidth) * 0.5625f) * 0.5f;
}