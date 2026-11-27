// Leaf: ?FormatTextForVWindow@CSubtitleRenderer@@ABEXABVC2DBoxF@@ABVCFontBank@@ABVCWideString@@AAV4@AAV2@@Z
// NF479 (Codex pass 29 name fixes): the word-wrap font is the private static ?PWordWrapFont@CSubtitleRenderer@@0PBVCFontBank@@B
//   (0x013B87AC; Ego_r / FableWin / Ego_d PUBLIC, was the free ?g_pWordWrapFont@@3PBVCFontBank@@B) and WordWrapText is the private
//   const ?WordWrapText@CSubtitleRenderer@@ABE?AVCWideString@@ABVC2DBoxF@@ABVCFontBank@@ABV2@@Z (0x0048DC8B; Ego_r + FableWin, was QBE).
// Naming wave 35c (caller respell): calls 0x0048DAE0 as the Ego_r.pdb template instance ??$GFLimit@MMM@@YIXAAMABM1@Z (was the
//   non-template ?GFLimit@@YIXAAMABM1@Z); the same-TU stand-in body is now that template's.
// Naming wave 18: PDisplayManager is GPDisplayManager (?GPDisplayManager@@3PAVCDisplayManager@@A); ChatGPT pass 10 audit, raw Ego_r public.
// CSubtitleRenderer::FormatTextForVWindow(const C2DBoxF& box, const CFontBank& font, const CWideString& text, CWideString& outText, C2DBoxF& outBox) const -- word-wrap the text for the box with this font, then size the output box: as wide as the widest wrapped line (clamped to the GUI def's text-box min/max width), centred on the virtual screen, and tall enough for the line count (one extra line unless the display flags say otherwise) above the box bottom
#pragma optimize("y", off)
#pragma optimize("s", on)
typedef unsigned short wchar;

class CWideString {
public:
    void* PStringData;
    ~CWideString();                                          // 0x0099B510
    const CWideString& operator=(const CWideString& other);        // 0x0099B7D0
    operator const wchar*() const;                           // 0x0099AF00 ??BCWideString@@QBEPBGXZ (Ego_r / FableWin ??BCWideString@@QBEPB_WXZ; NF93 q11296, was GetBuffer)
    bool operator!=(const wchar* text) const;                // 0x0099B150
};

class C2DBoxF { public: float TLX, TLY, BRX, BRY; };
class C2DExtentsI { public: long X, Y; };
class C2DExtentsF {
public:
    float Width;
    float Height;
};

float __fastcall GFAXToVX(float);                           // 0x009E1DA0
float __fastcall GFAYToVY(float y);                                    // 0x009E1DD0 ?GFAYToVY@@YIMM@Z (Ego_r + FableWin; NF285, was the __stdcall stand-in GFAYToVY_009e1dd0)
extern double g_VirtualScreenWidth;                          // 0x013961E8
template <class T, class TMin, class TMax> void __fastcall GFLimit(T& value, const TMin& minimum, const TMax& maximum);   // 0x0048DAE0 <float,float,float>

class CFontBank {
public:
    virtual ~CFontBank();
    virtual C2DExtentsF GetStringExtents(const wchar* text, bool wrap) const;
    virtual float GetMaxHeight() const;                      // slot 2 (+0x08)
    C2DExtentsF GetVStringExtents(const CWideString& text) const;   // 0x00437210
    float GetVFontMaxHeight() const { return GFAYToVY(GetMaxHeight()); }
};

void __fastcall WordWrap_SetOption(unsigned int option, unsigned int value);   // ?WordWrap_SetOption@@YIXII@Z (Ego_r.pdb + FableWin.pdb; name-fix 2026-11-12) // 0x00A0FDB0
void __fastcall WordWrap_SetCallback(unsigned int (__fastcall* charWidth)(wchar),
                                     unsigned int (__fastcall* lineHeight)());   // 0x00A0FD90

// NF35 (2026-11-16): the local CWStringParser model replaced by engine/CWStringParser.h (Ego_r + FableWin ??0CWStringParser@@QAE@PB_W@Z,
//   ?SkipWhitespace@CWStringParser@@QAEJXZ, ?Init@CWStringParser@@QAEXPB_W@Z; wide text is unsigned short, the retail PBG spelling).
#define FABLE_RETAIL_CWSTRINGPARSER_READUNTIL
#define FABLE_RETAIL_CWSTRINGPARSER_INIT
#include "engine/CWStringParser.h"                     // ctor 0x00437260, Init 0x00A0C8E0, SkipWhitespace 0x00A0C910, ReadAsStringUntilCharacter 0x00A0CCD0

class CDisplayManager { public:
    const C2DExtentsI GetRenderTargetDimensions();           // 0x009BEDC0
};
extern CDisplayManager* GPDisplayManager;                   // 0x013B8390

class CPlayerGuiDef { public:
    unsigned char _pad000[0x2ac];
    float TextBoxMaxWidth;                                   // +0x2AC (PDB +0x2C0)
    float TextBoxMinWidth;                                   // +0x2B0 (PDB +0x2C4)
};
class CPlayerGui {
public:
    static const CPlayerGuiDef& __fastcall PeekGuiDef();   // 0x00438570
};                // 0x00438570

class CSubtitleRenderer {
public:
    void*          __vftable;                                // +0x00
    unsigned char  _pad04[0x7c - 0x04];
    unsigned long  DisplayFlags;                             // +0x7C
    // Ego_r.pdb ?GetCharWidthW@CSubtitleRenderer@@CII_W@Z: a static __fastcall (private in the PDB; declared as the
    // landed 0x0048E012 declares it so both files bind the same symbol). Only its address is taken here.
    static unsigned int __fastcall GetCharWidthW(wchar character);   // 0x0048E012
    private:
    static const CFontBank* PWordWrapFont;                   // 0x013B87AC
    CWideString WordWrapText(const C2DBoxF& box, const CFontBank& font, const CWideString& text) const;   // 0x0048DC8B
    void FormatTextForVWindow(const C2DBoxF& box, const CFontBank& font, const CWideString& text, CWideString& outText,
                              C2DBoxF& outBox) const;
    public:
};

extern "C" int StandInSink(int, int);
#pragma auto_inline(off)
template <class T, class TMin, class TMax> void __fastcall GFLimit(T&, const TMin& c1, const TMax& c2)
{
    int hc = 0;
    { const unsigned char* b = (const unsigned char*)&c1; for (unsigned i = 0; i < sizeof(float); ++i) hc += b[i]; }
    { const unsigned char* b = (const unsigned char*)&c2; for (unsigned i = 0; i < sizeof(float); ++i) hc += b[i]; }
    StandInSink(hc, 10);
}
#pragma auto_inline(on)
void CSubtitleRenderer::FormatTextForVWindow(const C2DBoxF& box, const CFontBank& font, const CWideString& text, CWideString& outText,
                                             C2DBoxF& outBox) const
{
    PWordWrapFont = &font;
    WordWrap_SetOption(0, 1);
    WordWrap_SetCallback(GetCharWidthW, 0);
    outText = WordWrapText(box, font, text);
    float maxWidth = 0.0f;
    long lines = 0;
    CWStringParser parser(text);
    parser.Init(outText);
    CWideString line = parser.ReadAsStringUntilCharacter(L'\n');
    while (line != L"") {
        float width = font.GetVStringExtents(line).Width;
        if (width > maxWidth)
            maxWidth = width;
        ++lines;
        parser.SkipWhitespace();
        line = parser.ReadAsStringUntilCharacter(L'\n');
    }
    if (!(DisplayFlags & 2) || (DisplayFlags & 0x28))
        ++lines;
    if (lines == 0)
        lines = 1;
    float scale = GPDisplayManager->GetRenderTargetDimensions().X * (1.0f / 640.0f);
    const CPlayerGuiDef* def = (&CPlayerGui::PeekGuiDef());
    GFLimit(maxWidth, GFAXToVX(def->TextBoxMinWidth) * scale, GFAXToVX(def->TextBoxMaxWidth) * scale);
    outBox = box;
    outBox.TLX = (float)((g_VirtualScreenWidth - maxWidth) * 0.5);
    outBox.BRX = outBox.TLX + maxWidth;
    outBox.TLY = outBox.BRY - font.GetVFontMaxHeight() * lines;
}