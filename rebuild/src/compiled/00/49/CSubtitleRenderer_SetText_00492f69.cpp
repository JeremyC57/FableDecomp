// CSubtitleRenderer::SetText(CFontBank*, const CWideString&, const CWideString&, const CWideString&, const CWideString&, unsigned long, float, bool, bool) -- the two-button overload: forwards to the full SetText with an empty third button text
// Leaf: SetText
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00492F69, 75 bytes. Callees: CWideString::CWideString(const wchar_t*) @
// 0x0099B6B0 (empty wide literal 0x0122D70C), the full CSubtitleRenderer::SetText (font, text, name, button1, button2,
// button3, flags, time, bool, bool) @ 0x00492973, CWideString::~CWideString @ 0x0099B510.
// Layout: engine/CSubtitleRenderer.h is the PDB view (Ego_r: 16-byte vectors, a 0x18-byte deque); retail has the VC6
// containers (0x28-byte deque, 12-byte vectors), so DisplayFlags sits at +0x7C (PDB +0x6C) and the cutscene bools at
// +0xDC.. (PDB +0xE4..). The header disagrees with the bytes, so the class is modelled locally with the PDB names.
// Shape: frame pointer with `push ecx` for the 4-byte string local and `leave` = #pragma optimize("y", off) + ("s",
// on); the empty string is a named local built before any argument push and re-addressed with `lea`.
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <stddef.h>                                // wchar_t (no /Zc:wchar_t in the retail build)

class CWideString {
public:
    void* PStringData;
    CWideString(const wchar_t* text);              // 0x0099B6B0
    ~CWideString();                                // 0x0099B510
};

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
    void SetText(CFontBank* font, const CWideString& text, const CWideString& nameText, const CWideString& button1Text,
                 const CWideString& button2Text, const CWideString& button3Text, unsigned long displayFlags,
                 float displayTime, bool updateWhilePaused, bool changePlayerMode);   // 0x00492973
    void SetText(CFontBank* font, const CWideString& text, const CWideString& nameText, const CWideString& button1Text,
                 const CWideString& button2Text, unsigned long displayFlags, float displayTime,
                 bool updateWhilePaused, bool changePlayerMode);
};

void CSubtitleRenderer::SetText(CFontBank* font, const CWideString& text, const CWideString& nameText, const CWideString& button1Text,
                                const CWideString& button2Text, unsigned long displayFlags, float displayTime,
                                bool updateWhilePaused, bool changePlayerMode)
{
    CWideString button3Text(L"");
    SetText(font, text, nameText, button1Text, button2Text, button3Text, displayFlags, displayTime, updateWhilePaused, changePlayerMode);
}