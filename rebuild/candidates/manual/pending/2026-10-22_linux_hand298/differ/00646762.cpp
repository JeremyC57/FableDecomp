// Grade: DIFFER(711v740) -- lane gap: retail rounds the bar end with a hand-written inline-assembly fistp block (0.5/1.0 frame constants, +1 correction), which also forces the ebp frame; this brief C++ attempt rounds with floor and its graphic/text selection order is not tuned
// NPlayerGui::CStatBar::DrawStatBarOnLeft(const CEnginePrimitivePrepareInfo& info, const CGraphicDataBank& bank, const C2DVector& pos, const C2DVector& textOffset, const CRGBColour& colour, unsigned char alpha, bool withText) -- place the left-anchored bar end at the rounded fill position, pick the change graphic (positive/negative arrow from the pending change list, or the stat-type default), build the value text (absolute value with the sign literal for the money type, the sign literal for gains), draw the stat box, draw the text mirrored to the right edge when asked, and draw the pending change markers interpolated by the frame fraction
// Leaf: DrawStatBarOnLeft@CStatBar
// Retail: Fable.exe (TLC, VC7.1) @ 0x00646762, 740 bytes, `ret 1Ch`, frame-pointer TU (`leave`).
// Callees: the stat bar's screen clamp 0x00643CDA (__fastcall), CWideString from long 0x00405F00 (__fastcall), __ftol2,
//   operator+(const wchar_t*, const CWideString&) 0x0099BFF0, CWideString::operator= 0x0099B7D0, ~CWideString 0x0099B510,
//   DrawStatBox (virtual slot 2), C2DVector to screen 0x009E21D0 (__fastcall), CFontBank::GetVStringExtents 0x00437210,
//   CDisplayManager::GetRenderTargetDimensions 0x009BEDC0, mirror helper 0x009E1DA0, CGameUI text draw 0x00438010, the
//   change-marker draw 0x00645F2E.
// Layout (descriptive, as the landed DrawStatBox 0x00645CE9 TU): def +0x4 (bar left offset +0x4bc, default graphic +0x2ec,
//   negative graphic +0x2f0, stat-type table +0x54c / +0x530 / +0x534 / +0x538 / +0x53c / +0x558), Value +0x10, Mode +0x28,
//   Fraction +0x2c, StatType +0x50, Effect +0x54, Graphic +0x58, Font +0x60, pending change list +0x64 (circular, data
//   +0x8..+0x24).
// Shape: retail rounds the bar end with a hand-written inline-assembly fistp block (0.5 and 1.0 frame constants, the
//   +1 correction when the rounded value plus one equals the input); this C++ form rounds with floor.
#include <math.h>
#pragma optimize("y", off)
struct C2DVector {
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
};
inline C2DVector operator+(const C2DVector& a, const C2DVector& b) { return C2DVector(a.X + b.X, a.Y + b.Y); }
struct CRGBColour { unsigned char R, G, B, A; };
class CWideString {
public:
    ~CWideString();                                                     // 0x0099B510
    CWideString& operator=(const CWideString& other);                   // 0x0099B7D0
    void* Data;
};
CWideString __fastcall LongToWideString(long value);                    // 0x00405F00
CWideString __fastcall operator+(const unsigned short* text, const CWideString& other);   // 0x0099BFF0
struct CEnginePrimitivePrepareInfo { unsigned long _pad0; float FrameFraction; };
struct CGraphicDataBank;
C2DVector __fastcall ClampStatBarPosition(const C2DVector& pos);       // 0x00643CDA
C2DVector __fastcall ToScreen(const C2DVector& pos);                   // 0x009E21D0
float __stdcall MirrorX(float width);                                   // 0x009E1DA0
struct C2DExtentsI { long Width, Height; };
struct CDisplayManager { C2DExtentsI GetRenderTargetDimensions(); };   // 0x009BEDC0
extern CDisplayManager* g_pDisplayManager_013b8390;                    // 0x013B8390
struct C2DExtentsF { float Width, Height; };
struct CFontBank { C2DExtentsF GetVStringExtents(const CWideString& text); };   // 0x00437210
struct CGameUI {
    void DrawText(const C2DVector& pos, const CWideString& text, CFontBank* font, const CRGBColour& colour, float scale, long layer, bool flag);   // 0x00438010
};
extern CGameUI* g_pGameUI;                                              // 0x013B8790
extern const unsigned short g_StatSignText[];                                  // 0x01231760
struct CStatTypeEntry { unsigned char _pad[0x10]; long Type; long Value; };
struct CStatBarDef {
    unsigned char   _pad000[0x2ec];
    long            PositiveGraphic;                                    // +0x2ec
    long            NegativeGraphic;                                    // +0x2f0
    unsigned char   _pad2f4[0x4bc - 0x2f4];
    float           BarLeftOffset;                                      // +0x4bc
    unsigned char   _pad4c0[0x530 - 0x4c0];
    long            StatTypeA;                                          // +0x530
    long            StatTypeB;                                          // +0x534
    long            GraphicA;                                           // +0x538
    long            GraphicB;                                           // +0x53c
    unsigned char   _pad540[0x54c - 0x540];
    CStatTypeEntry* StatTypes;                                          // +0x54c
    unsigned char   _pad550[0x558 - 0x550];
    CStatTypeEntry* DefaultGraphic;                                     // +0x558
};
struct CChangeNode {
    CChangeNode* Next;
    CChangeNode* Prev;
    C2DVector    From;                                                  // +0x08
    C2DVector    To;                                                    // +0x10
    C2DVector    A;                                                     // +0x18
    long         Amount;                                                // +0x1c within A's second word
    C2DVector    B;                                                     // +0x20
};
namespace NPlayerGui {
struct CStatBar {
    virtual void v0();
    virtual void v1();
    virtual void DrawStatBox(const CEnginePrimitivePrepareInfo& info, const CGraphicDataBank& bank, const C2DVector& pos, long type, long effect, long graphic, const CWideString& text, unsigned char alpha, bool withText, bool flag);
    CStatBarDef*  Def;                                                  // +0x04
    unsigned char _pad08[0x10 - 0x08];
    float         Value;                                                // +0x10
    unsigned char _pad14[0x28 - 0x14];
    long          Mode;                                                 // +0x28
    float         Fraction;                                             // +0x2c
    unsigned char _pad30[0x50 - 0x30];
    long          StatType;                                             // +0x50
    long          Effect;                                               // +0x54
    long          Graphic;                                              // +0x58
    unsigned char _pad5c[0x60 - 0x5c];
    CFontBank*    Font;                                                 // +0x60
    CChangeNode*  Changes;                                              // +0x64
    void DrawChangeMarker(const C2DVector& pos, const C2DVector& a, const C2DVector& b);   // 0x00645F2E
    void DrawStatBarOnLeft(const CEnginePrimitivePrepareInfo& info, const CGraphicDataBank& bank, const C2DVector& pos, const C2DVector& textOffset, const CRGBColour& colour, unsigned char alpha, bool withText);
};

void CStatBar::DrawStatBarOnLeft(const CEnginePrimitivePrepareInfo& info, const CGraphicDataBank& bank, const C2DVector& pos, const C2DVector& textOffset, const CRGBColour& colour, unsigned char alpha, bool withText)
{
    float left = -Def->BarLeftOffset;
    float x = (pos.X - left) * Fraction + left;
    long rounded = (long)floorf(x + 0.5f);
    C2DVector barEnd = ClampStatBarPosition(C2DVector((float)rounded, pos.Y));
    C2DVector textPos = barEnd + textOffset;
    CWideString text = LongToWideString((long)Value);
    long graphic = Graphic;
    if (graphic == Def->PositiveGraphic) {
        graphic = 0;
        if (Changes->Next != Changes) {
            long amount = Changes->Next->Amount;
            if (amount > 0)
                graphic = Def->PositiveGraphic;
            else if (amount < 0)
                graphic = Def->NegativeGraphic;
        }
    } else {
        if (!(Fraction < 1.0f)) {
            if (graphic == 0) {
                if (StatType == Def->StatTypes->Type)
                    Graphic = Def->DefaultGraphic->Type;
                else if (StatType == Def->StatTypeA)
                    Graphic = Def->GraphicA;
                else if (StatType == Def->StatTypeB)
                    Graphic = Def->GraphicB;
                graphic = Graphic;
            }
        } else if (Fraction < 0.25f) {
            graphic = 0;
            Graphic = 0;
        }
        if (StatType == Def->StatTypes->Value)
            text = g_StatSignText + LongToWideString((long)fabsf(Value));
        if (Value > 0.0f)
            text = g_StatSignText + text;
    }
    DrawStatBox(info, bank, barEnd, StatType, Effect, graphic, text, alpha, withText, false);
    C2DVector screen = ToScreen(textPos);
    if (withText) {
        float width = Font->GetVStringExtents(text).Width;
        screen.X = MirrorX((float)g_pDisplayManager_013b8390->GetRenderTargetDimensions().Width) - screen.X - width;
    }
    CRGBColour c;
    c.R = colour.R;
    c.G = colour.G;
    c.B = colour.B;
    c.A = alpha;
    g_pGameUI->DrawText(screen, text, Font, c, 1.0f, 0x43, false);
    if (Mode == 0 || Mode == 3) {
        float t = info.FrameFraction;
        for (CChangeNode* node = Changes->Next; node != Changes; node = node->Next) {
            C2DVector p((node->To.X - node->From.X) * t + node->From.X + barEnd.X, (node->To.Y - node->From.Y) * t + node->From.Y + barEnd.Y);
            DrawChangeMarker(ToScreen(p), node->A, node->B);
        }
    }
}
}
