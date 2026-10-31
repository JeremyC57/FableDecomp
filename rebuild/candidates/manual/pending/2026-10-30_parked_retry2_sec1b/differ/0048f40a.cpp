// CSubtitleRenderer::DrawTextEmbeddedHighlights(long, const C2DVector& offset) -- draw every embedded highlight whose character has been revealed, at its offset from the (virtual-coordinate) text origin, in the GUI def's highlight colour
// Grade: DIFFER(258v258) sec1b: GFACoordToVCoord respelled to the PDB `(const C2DVector&, C2DVector&)` and called on a `C2DVector(x, y) + Offset` temporary: the x87 tail now matches retail's by-value-parameter shape (fld st(1); fstp X; fst Y; fstp; fstp) but the sums run Y-first and the two leas still come after the fadds. A `C2DVector(x + o.X, y + o.Y)` temporary gives 252v258 (no param copies). Next: an operator+ argument order that sums X first. Earlier: DIFFER(258v258) -- scheduling only: retail places `lea edx,[ebp-18h]` / `lea ecx,[ebp-10h]` (the GFACoordToVCoord arguments) between the Y `fld`/`fadd`, ours after them
#pragma optimize("y", off)
#pragma optimize("s", on)
// Leaf: DrawTextEmbeddedHighlights
// Retail: Fable.exe (TLC, VC7.1) @ 0x0048F40A, 258 bytes, `ret 8` (the first argument is never read).
// Callees: PeekGuiDef 0x00438570 (__fastcall), GFAYToVY 0x009E1E40 / GFAXToVX 0x009E1E10 (__fastcall, float on the
//   stack), GFACoordToVCoord(const C2DVector*, C2DVector*) 0x009E21D0 (__fastcall),
//   CSubtitleRenderer::Draw2DText 0x0048F1F4 (landed). Float constant 2.0f 0x0122DCDC.
// Layout: the retail CSubtitleRenderer of the landed siblings (VC6 containers, flags at Ego_r.pdb - 8): TextCharactersShowing
//   +0x24, PFont +0x80, EmbeddedHighlights +0xC4 (12-byte vector of 20-byte CEmbeddedHighlight {Text, CharIndex, Offset,
//   Split} as in Ego_r.pdb), AllCharactersDisplayed +0xDD. The GUI def's highlight colour sits at +0x5BC (name
//   descriptive).
// Shape: the frame-pointer size TU of the landed rows (`push 14h ; pop ecx ; idiv` sizes). The vector is re-read for
//   every element access through the iterator `*(begin() + i)`; the Y conversion is a named float local parked in the
//   dead `offset` slot; the screen position is built by the by-value C2DVector constructor over a reference to the
//   record's Offset (`lea eax,[edi+eax+8]`, X summed first). Tried: the plain member sums (Y first), member and free
//   operator+ (same as this file), the screen local declared first, a temporary position (stores reordered).
struct C2DVector {
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector operator+(const C2DVector& o) const { return C2DVector(X + o.X, Y + o.Y); }
};
struct CRGBColour { unsigned char B, G, R, A; };
class CWideString { public: void* PStringData; };
class CFontBank;
class CGraphicDataBank;

struct CPlayerGuiDef {
    unsigned char _pad[0x5bc];
    CRGBColour    SubtitleHighlightColour;         // +0x5BC
};
CPlayerGuiDef* __fastcall PeekGuiDef();            // 0x00438570
float __fastcall GFAYToVY(float y);                // 0x009E1E40
float __fastcall GFAXToVX(float x);                // 0x009E1E10
void __fastcall GFACoordToVCoord(const C2DVector& in, C2DVector& out);   // 0x009E21D0 (Ego_r ?GFACoordToVCoord@@YIXABVC2DVector@@AAV1@@Z)

template <class T> struct CRetailVector {          // VC6 / STLport vector: 12 bytes
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    T* begin() { return m_pFirst; }
    bool empty() const { return m_pFirst == m_pLast; }
    unsigned long size() const { return m_pLast - m_pFirst; }
    T& operator[](unsigned long n) { return *(begin() + n); }
};

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
struct C2DBoxF { float TLX, TLY, BRX, BRY; };

class CSubtitleRenderer {
public:
    struct CEmbeddedHighlight {
        CWideString Text;                          // +0x00
        long        CharIndex;                     // +0x04
        C2DVector   Offset;                        // +0x08
        bool        Split;                         // +0x10
    };
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
    CRetailVector<long> PrimitiveHandles;          // +0x94
    CRetailVector<long> ButtonPrimitiveHandles;    // +0xA0
    CRetailVector<long> EmbeddedPrimitiveHandles;  // +0xAC
    CRetailVector<long> EmbeddedGraphics;          // +0xB8
    CRetailVector<CEmbeddedHighlight> EmbeddedHighlights;   // +0xC4
    CRetailVector<CWideString> EmbeddedHighlightActionNames; // +0xD0
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
                    bool unused, float scaleX, float scaleY, bool raise);   // 0x0048F1F4
    void DrawTextEmbeddedHighlights(long unused, const C2DVector& offset);
};

void CSubtitleRenderer::DrawTextEmbeddedHighlights(long unused, const C2DVector& offset)
{
    if (EmbeddedHighlights.empty())
        return;
    CPlayerGuiDef* def = PeekGuiDef();
    for (unsigned long i = 0; i < EmbeddedHighlights.size(); ++i) {
        if (AllCharactersDisplayed || TextCharactersShowing > EmbeddedHighlights[i].CharIndex) {
            float y = GFAYToVY(offset.Y);
            float x = GFAXToVX(offset.X);
            C2DVector screen;
            GFACoordToVCoord(C2DVector(x, y) + EmbeddedHighlights[i].Offset, screen);
            Draw2DText(screen, EmbeddedHighlights[i].Text, PFont, def->SubtitleHighlightColour, true, 2.0f, 1.0f, true);
        }
    }
}
