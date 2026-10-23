// Grade: DIFFER(906v927) -- three residues: CWideString/float slot order (visible vs width/y, line vs visibleLine), the centring x87 form (retail fld st0; fld st1; fsubr [screenH]), and the inline ==L"" materialised as a bool (retail branches straight off wcscmp/EqualsLen)
// CSubtitleRenderer::DrawTextPrimitives(const CEnginePrimitivePrepareInfo& info, const C2DBoxF& vWindow, const C2DBoxF& vTelevisionWindow) -- lay out the subtitle text box from the GUI def (above the black bar), word-wrap the text into it (vertically centred for game-info text), and draw the revealed part line by line (centred unless left-justified, grey for "(HC)" text), then the embedded graphics and highlights; returns the text box
// Leaf: DrawTextPrimitives
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <stddef.h>
extern "C" int __cdecl strncmp(const char* a, const char* b, unsigned int count);   // 0x00BFEAF8
extern "C" int __cdecl wcscmp(const wchar_t* a, const wchar_t* b);                    // 0x00BFE94C

struct C2DVector {
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
};
struct C2DBoxF {
    float TLX, TLY, BRX, BRY;
    C2DBoxF() { }
    C2DBoxF(const C2DBoxF& o) : TLX(o.TLX), TLY(o.TLY), BRX(o.BRX), BRY(o.BRY) { }
    C2DBoxF(float tlx, float tly, float brx, float bry) : TLX(tlx), TLY(tly), BRX(brx), BRY(bry) { }
};
struct C2DExtentsI { long Width, Height; };
struct C2DExtentsF { float Width, Height; };
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
struct CCharStringData {
    const char*   Text;                                  // +0x00
    long          Length;                                // +0x04
};
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
    const char* GetPString() const { return PStringData == 0 ? "" : PStringData->Text; }
};
static inline bool BeginsWith(const CCharString& text, const CCharString& prefix)
{
    const char* prefixText;
    long prefixLength;
    if (prefix.PStringData == 0) {
        prefixLength = 0;
        prefixText = "";
    } else {
        prefixLength = prefix.PStringData->Length;
        prefixText = prefix.PStringData->Text;
    }
    return strncmp(text.GetPString(), prefixText, prefixLength) ? false : true;
}
struct CWideStringData { wchar_t* Begin; wchar_t* End; };
bool __cdecl operator==(const CWideStringData& data, const wchar_t* text);   // 0x00404280
class CWideString {
public:
    CWideStringData* PStringData;
    CWideString();                                       // 0x0099AED0
    ~CWideString();                                      // 0x0099B510
    CWideString& operator=(const CWideString& other);    // 0x0099B7D0
    CCharString ConvertToCharString() const;             // 0x0099B2C0
    bool operator!=(const wchar_t* text) const;          // 0x0099B150
    operator const wchar_t*() const;                     // 0x0099AF00
    __forceinline bool operator==(const wchar_t* text) const
    {
        if (PStringData == 0)
            return wcscmp(L"", text) == 0;
        return *PStringData == text;
    }
};
class CWStringParser {
public:
    void*         __vftable;
    const wchar_t* String;
    unsigned long StringPos;
    unsigned long Line;
    CWStringParser(const wchar_t* text);                 // 0x00437260
    ~CWStringParser();                                   // 0x0099A2E0 (the base destructor)
    CWideString ReadAsStringUntilCharacter(wchar_t stop);   // 0x00A0CCD0
    long SkipWhitespace();                               // 0x00A0C910
};
class CFontBank {
public:
    virtual void v00();
    virtual void v01();
    virtual float GetVLineHeight() const;                // slot 2 (+0x08)
    C2DExtentsF GetVStringExtents(const CWideString& text) const;   // 0x00437210
};
float __fastcall GFAXToVX(float x);                      // 0x009E1DA0
float __fastcall GFAYToVY(float y);                      // 0x009E1DD0
extern double g_screenHeight;                            // 0x013961F0
struct CDisplayManager { C2DExtentsI GetRenderTargetDimensions() const; };   // 0x009BEDC0
extern CDisplayManager* g_pDisplayManager;               // 0x013B8390
struct CDisplayCaps { void SetVViewport(const C2DBoxF& box); };   // 0x00A0B560
extern CDisplayCaps* g_pDisplayCaps;                     // 0x013B8384
extern bool g_LeftJustifySubtitles;                      // 0x013B861B
struct CPlayerGuiDef {
    unsigned char _pad000[0x5a4];
    C2DVector     TextBoxTextAreaTLPos;                  // +0x5a4 (PDB +0x5e0)
    C2DVector     TextBoxTextAreaBRPos;                  // +0x5ac
    CRGBColour    TextBoxTextColour;                     // +0x5b4
    CRGBColour    TextBoxTextGameInfoColour;             // +0x5b8
};
const CPlayerGuiDef* __fastcall PeekGuiDef();            // 0x00438570
class CEnginePrimitivePrepareInfo;
static inline void CentreVertically(C2DBoxF& box, float height)
{
    box.TLY = (g_screenHeight - height) * 0.5;
    box.BRY = box.TLY + height;
}

class CSubtitleRenderer {
public:
    void*         __vftable;                             // +0x00
    C2DBoxF       VWindow;                               // +0x04
    C2DBoxF       VTelevisionWindow;                     // +0x14
    unsigned char _pad24[0x5c - 0x24];
    CWideString   Text;                                  // +0x5c
    unsigned char _pad60[0x7c - 0x60];
    unsigned long DisplayFlags;                          // +0x7c
    CFontBank*    PFont;                                 // +0x80
    float GetBlackBarHeight() const;                     // 0x0048DBC3
    bool GetVisibleText(const CWideString& text, CWideString& visible);   // 0x0048DE8A
    void FormatTextForVWindow(const C2DBoxF& window, CFontBank* font, const CWideString& text, CWideString& formatted,
                              C2DBoxF& textBox);         // 0x0048F50C
    void DrawTextEmbeddedGraphics(const CEnginePrimitivePrepareInfo& info, const C2DBoxF& box);    // 0x0048F230
    void DrawTextEmbeddedHighlights(const CEnginePrimitivePrepareInfo& info, const C2DBoxF& box);  // 0x0048F40A
    void Draw2DText(const C2DVector& pos, const CWideString& text, CFontBank* font, const CRGBColour& colour,
                    bool flag, float unused, float scale, bool raiseLayer);                     // 0x0048F1F4
    C2DBoxF DrawTextPrimitives(const CEnginePrimitivePrepareInfo& info, const C2DBoxF& vWindow, const C2DBoxF& vTelevisionWindow);
};

C2DBoxF CSubtitleRenderer::DrawTextPrimitives(const CEnginePrimitivePrepareInfo& info, const C2DBoxF& vWindow,
                                              const C2DBoxF& vTelevisionWindow)
{
    const CPlayerGuiDef* def = PeekGuiDef();
    CRGBColour colour(0xff, 0xff, 0xff, 0xff);
    if (DisplayFlags & 8)
        colour = def->TextBoxTextGameInfoColour;
    if (BeginsWith(Text.ConvertToCharString(), "(HC)"))
        colour = CRGBColour(0x80, 0x80, 0x80, 0xff);
    CWideString formatted;
    float width = (float)g_pDisplayManager->GetRenderTargetDimensions().Width;
    C2DBoxF textBox;
    FormatTextForVWindow(C2DBoxF(GFAXToVX(def->TextBoxTextAreaTLPos.X),
                                 g_screenHeight - GFAYToVY(GetBlackBarHeight()) - GFAYToVY(def->TextBoxTextAreaTLPos.Y),
                                 GFAXToVX(width + def->TextBoxTextAreaBRPos.X - 640.0f),
                                 g_screenHeight - GFAYToVY(GetBlackBarHeight()) - GFAYToVY(def->TextBoxTextAreaBRPos.Y)),
                         PFont, Text, formatted, textBox);
    if (DisplayFlags & 8)
        CentreVertically(textBox, textBox.BRY - textBox.TLY);
    g_pDisplayCaps->SetVViewport(textBox);
    CWideString visible;
    GetVisibleText(formatted, visible);
    float y = 0.0f;
    CWStringParser formattedParser(formatted);
    CWStringParser visibleParser(visible);
    CWideString line = formattedParser.ReadAsStringUntilCharacter(10);
    while (line != L"") {
        CWideString visibleLine = visibleParser.ReadAsStringUntilCharacter(10);
        if (visibleLine == L"")
            break;
        float x = 0.0f;
        if (!g_LeftJustifySubtitles)
            x = (textBox.BRX - textBox.TLX - PFont->GetVStringExtents(line).Width) * 0.5f;
        Draw2DText(C2DVector(textBox.TLX + x, y + textBox.TLY), visibleLine, PFont, colour, true, 2.0f, 1.0f, false);
        y += GFAYToVY(PFont->GetVLineHeight());
        formattedParser.SkipWhitespace();
        visibleParser.SkipWhitespace();
        line = formattedParser.ReadAsStringUntilCharacter(10);
    }
    DrawTextEmbeddedGraphics(info, textBox);
    DrawTextEmbeddedHighlights(info, textBox);
    g_pDisplayCaps->SetVViewport(vWindow);
    return textBox;
}
