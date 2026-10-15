// CSubtitleRenderer::GetTextOffsetFromEmbeddedTagGraphic(long) const -- the text offset to use next to an embedded controller graphic: the click offset for the thumbstick-click graphics, the trigger offset for the triggers, else the plain graphic offset
// Grade: DIFFER(ours v retail) -- 79v79: block placement only: retail places the thumb-click return (`add eax,6A4h ; jmp`) right after its `||` pair (`je A ; cmp ; jne next ; A:`), the lane moves it below the default return (`je ; je` to the end); else-if returns, nested `!=` guard, a result pointer local and a ternary chain all give the lane layout
// Leaf: GetTextOffsetFromEmbeddedTagGraphic
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0048DC3C, 79 bytes. Callee: PeekGuiDef @ 0x00438570 (__fastcall, no
// arguments, landed), returning the CPlayerGuiDef. `this` is not used.
// Def layout (PDB CPlayerGuiDef, retail 0x3C lower): ControllerThumbLeftGraphic +0x66C, ThumbRight +0x670,
// ThumbClickLeft +0x674, ThumbClickRight +0x678, TriggerLeft +0x684, TriggerRight +0x688, ControllerGraphicTextOffset
// +0x69C, ControllerClickGraphicTextOffset +0x6A4, ControllerTriggerGraphicTextOffset +0x6AC.
// Layout: engine/CSubtitleRenderer.h is the PDB view (Ego_r: 16-byte vectors, a 0x18-byte deque); retail has the VC6
// containers (0x28-byte deque, 12-byte vectors), so DisplayFlags sits at +0x7C (PDB +0x6C) and the cutscene bools at
// +0xDC.. (PDB +0xE4..). The header disagrees with the bytes, so the class is modelled locally with the PDB names.
// Shape: an if/else-if chain of `||` pairs on the graphic index (`cmp ecx,[eax+n]`), each returning the address of a
// C2DVector member (`add eax,n`) through one shared `ret 4` (#pragma optimize("s", on)); the thumbstick pair shares the
// default return.
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
    const C2DVector& GetTextOffsetFromEmbeddedTagGraphic(long graphic) const;
};

struct CPlayerGuiDef {
    unsigned char _pad000[0x66C];
    long          ControllerThumbLeftGraphic;          // +0x66C
    long          ControllerThumbRightGraphic;         // +0x670
    long          ControllerThumbClickLeftGraphic;     // +0x674
    long          ControllerThumbClickRightGraphic;    // +0x678
    long          ControllerBackGraphic;               // +0x67C
    long          ControllerStartGraphic;              // +0x680
    long          ControllerTriggerLeftGraphic;        // +0x684
    long          ControllerTriggerRightGraphic;       // +0x688
    long          ControllerTriggerBlackGraphic;       // +0x68C
    long          ControllerTriggerWhiteGraphic;       // +0x690
    C2DVector     ControllerGraphicOffset;             // +0x694
    C2DVector     ControllerGraphicTextOffset;         // +0x69C
    C2DVector     ControllerClickGraphicTextOffset;    // +0x6A4
    C2DVector     ControllerTriggerGraphicTextOffset;  // +0x6AC
};
CPlayerGuiDef* __fastcall PeekGuiDef();                 // 0x00438570

const C2DVector& CSubtitleRenderer::GetTextOffsetFromEmbeddedTagGraphic(long graphic) const
{
    const CPlayerGuiDef* def = PeekGuiDef();
    if (graphic == def->ControllerThumbLeftGraphic || graphic == def->ControllerThumbRightGraphic)
        return def->ControllerGraphicTextOffset;
    else if (graphic == def->ControllerThumbClickLeftGraphic || graphic == def->ControllerThumbClickRightGraphic)
        return def->ControllerClickGraphicTextOffset;
    else if (graphic == def->ControllerTriggerLeftGraphic || graphic == def->ControllerTriggerRightGraphic)
        return def->ControllerTriggerGraphicTextOffset;
    return def->ControllerGraphicTextOffset;
}
