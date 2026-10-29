// Leaf: FormatTextForVWindow
// CSubtitleRenderer::FormatTextForVWindow(const C2DBoxF& box, const CFontBank& font, const CWideString& text, CWideString& outText, C2DBoxF& outBox) const -- word-wrap the text for the box with this font, then size the output box: as wide as the widest wrapped line (clamped to the GUI def's text-box min/max width), centred on the virtual screen, and tall enough for the line count (one extra line unless the display flags say otherwise) above the box bottom
#pragma optimize("y", off)
#pragma optimize("s", on)
typedef unsigned short wchar;

class CWideString {
public:
    void* PStringData;
    ~CWideString();                                          // 0x0099B510
    const CWideString& operator=(const CWideString& other);        // 0x0099B7D0
    wchar* GetBuffer() const;                                // 0x0099AF00
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
float __stdcall GFAYToVY_009e1dd0(float y);                           // 0x009E1DD0
extern double g_VirtualScreenWidth;                          // 0x013961E8
void __fastcall GFLimit(float& value, const float& minimum, const float& maximum);   // 0x0048DAE0

class CFontBank {
public:
    virtual ~CFontBank();
    virtual C2DExtentsF GetStringExtents(const wchar* text, bool wrap) const;
    virtual float GetMaxHeight() const;                      // slot 2 (+0x08)
    C2DExtentsF GetVStringExtents(const CWideString& text) const;   // 0x00437210
    float GetVFontMaxHeight() const { return GFAYToVY_009e1dd0(GetMaxHeight()); }
};
extern const CFontBank* g_pWordWrapFont;                     // 0x013B87AC

void __fastcall WordWrap_SetOption(long option, long value); // 0x00A0FDB0
void __fastcall WordWrap_SetCallback(unsigned int (__fastcall* charWidth)(wchar),
                                     unsigned int (__fastcall* lineHeight)());   // 0x00A0FD90

class CBaseClass {
public:
    virtual ~CBaseClass();                                       // 0x0099A2E0
};
struct CWStringParser : CBaseClass {
    wchar*        String;                                    // +0x04
    unsigned long StringPos;                                 // +0x08
    unsigned long Line;                                      // +0x0C
    CWStringParser(wchar* text);                             // 0x00437260
    void Set(wchar* text);                                   // 0x00A0C8E0
    void SkipWhitespace();                                   // 0x00A0C910
    CWideString ReadAsStringUntilCharacter(wchar stop);      // 0x00A0CCD0
};

class CDisplayManager { public:
    const C2DExtentsI GetRenderTargetDimensions();           // 0x009BEDC0
};
extern CDisplayManager* g_pDisplayManager;                   // 0x013B8390

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
    CWideString WordWrapText(const C2DBoxF& box, const CFontBank& font, const CWideString& text) const;   // 0x0048DC8B
    void FormatTextForVWindow(const C2DBoxF& box, const CFontBank& font, const CWideString& text, CWideString& outText,
                              C2DBoxF& outBox) const;
};

extern "C" int StandInSink(int, int);
#pragma auto_inline(off)
void __fastcall GFLimit(float&, const float& c1, const float& c2)
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
    g_pWordWrapFont = &font;
    WordWrap_SetOption(0, 1);
    WordWrap_SetCallback(GetCharWidthW, 0);
    outText = WordWrapText(box, font, text);
    float maxWidth = 0.0f;
    long lines = 0;
    CWStringParser parser(text.GetBuffer());
    parser.Set(outText.GetBuffer());
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
    float scale = g_pDisplayManager->GetRenderTargetDimensions().X * (1.0f / 640.0f);
    const CPlayerGuiDef* def = (&CPlayerGui::PeekGuiDef());
    GFLimit(maxWidth, GFAXToVX(def->TextBoxMinWidth) * scale, GFAXToVX(def->TextBoxMaxWidth) * scale);
    outBox = box;
    outBox.TLX = (float)((g_VirtualScreenWidth - maxWidth) * 0.5);
    outBox.BRX = outBox.TLX + maxWidth;
    outBox.TLY = outBox.BRY - font.GetVFontMaxHeight() * lines;
}