// CSubtitleRenderer::ProcessEmbeddedHighlightTags() -- turn every <tag> in the subtitle text into a quoted highlight: the tags become 'tag' in the text and in the highlight action-name list; each name is then located in the text (after the previous match of the same name) as an embedded highlight; once the text is formatted for the GUI def's text area, highlights that the word wrap split over a line break get a second (split) highlight for the part on the next line, every highlight's character index is moved past the inserted line breaks, and its offset is set from the font extents of the text up to it (the last line), raised by the line count, and (unless the text is left aligned) shifted to the centred line start
// Grade: DIFFER(2240v2223) -- biased /Os frame, tag loop, action-name search, highlight split / push_back loops, the StartsWith strncmp expansion and the per-highlight extents / offset / centring tail are retail-shaped (slot-agnostic 113 lines); residue: VC7.1 peels the first compare of the newline-count loop against the known 0 (retail compares the memory slot every pass) and allocates five extra frame slots for the box / extents temporaries (frame 0xAC vs 0xA0)
// Next idea: give the box / extents temporaries retail's three slots (-0x28 / -0x20 / -0x18): try the C2DBoxF and extents values as one reused local or as direct call arguments, and write the newline count so the counter lives in memory before the loop
// Leaf: ProcessEmbeddedHighlightTags
// Retail: Fable.exe (TLC, VC7.1) @ 0x00491AD2, 2223 bytes, thiscall plain `ret`; the frame-pointer /Os TU of the CSubtitleRenderer
//   neighbours (`push ebp ; lea ebp,[esp-78h] ; sub esp,0A0h`, `push 14h ; pop ecx ; idiv` vector sizes, `leave`).
// Callees: vector<CEmbeddedHighlight>::erase 0x004940AC / push_back 0x004942C3 / ~vector 0x00442870, vector<CWideString>::push_back
//   0x004128A0, CWideString() 0x0099AED0 / (const CWideString&) 0x0099B720 / ~ 0x0099B510 / operator= 0x0099B7D0 / FindCharPos 0x0099B070
//   / GetSubString 0x0099BDB0 / operator!=(const wchar_t*) 0x0099B150 / GetLength 0x0099B220 / GetAsCharArray 0x0099AF10 / operator const
//   wchar_t* 0x0099AF00 / ToCharString 0x0099B2C0, operator+ (CWideString, const wchar_t*) 0x0099BF30 / (CWideString, CWideString)
//   0x0099BE70 / (const wchar_t*, CWideString) 0x0099BFF0, STLport basic_string operator== 0x0040F0A0 / (string, text) 0x00404280,
//   CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0, _wcsstr 0x00BFEB2E, _wcscmp 0x00BFE94C, _strncmp 0x00BFEAF8,
//   CPlayerGui::PeekGuiDef 0x00438570, CDisplayManager::GetRenderTargetDimensions 0x009BEDC0 (GPDisplayManager 0x013B8390),
//   CSubtitleRenderer::GetBlackBarHeight 0x0048DBC3 / FormatTextForVWindow 0x0048F50C, GFAYToVY 0x009E1DD0 / GFAXToVX 0x009E1DA0 /
//   GFVXToAX 0x009E1E10, GetNumberOfCharactersOfTypeInString 0x0048DBF9, CFontBank::GetStringExtents(const CWideString&, bool)
//   0x0048DAA2 / GetStringWidth 0x0048DA62 / slot 2 GetFontMaxHeight, CWStringParser(const wchar_t*) 0x00437260 /
//   ReadAsStringUntilCharacter 0x00A0CCD0 / SkipWhitespace 0x00A0C910, ~CBaseClass 0x0099A2E0.
// Data: L"'" 0x012384D0, L"" 0x0122D70C, "(HC)" 0x0123172C, 640.0f 0x01238168, 0.0f 0x0122DEDC, 0.5f 0x0122F59C, the virtual screen
//   height double 0x013961F0, g_FableLeftAlignText_013B861B.
// Layout: engine/CSubtitleRenderer.h (retail STLport offsets): Text +0x5C, PFont +0x80, EmbeddedHighlights +0xC4 (20-byte Ego_r
//   CEmbeddedHighlight {Text, CharIndex, Offset, Split}), EmbeddedHighlightActionNames +0xD0. CPlayerGuiDef TextBoxTextAreaTLPos /
//   BRPos at retail +0x5A4 / +0x5AC (Ego_r +0x5E0 / +0x5E8, the 0x3C container shift of the landed neighbours).
#pragma optimize("s", on)
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING_TEXT_EQ
#define FABLE_RETAIL_CWIDESTRING_FORCEINLINE_EQ
#define FABLE_RETAIL_CWIDESTRING_SUBSTRING
#define FABLE_RETAIL_CCHARSTRING_STARTSWITH
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CFONTBANK_STRING_HELPERS
#define FABLE_RETAIL_C2DBOXF
#include "fable_string.h"
#include "engine/CFontBank.h"
#include "engine/CBaseClass.h"
#include "engine/C2DVector.h"
#include "engine/C2DBoxF.h"
#define CSubtitleRenderer CSubtitleRendererLayout
#include "engine/CSubtitleRenderer.h"
#undef CSubtitleRenderer

class C2DExtentsI { public: long X, Y; };
class CDisplayManager { public:
    const C2DExtentsI GetRenderTargetDimensions();           // 0x009BEDC0
};
extern CDisplayManager* GPDisplayManager;                   // 0x013B8390
extern double g_screenHeight;                               // 0x013961F0
extern fable_i8 g_FableLeftAlignText_013B861B;
float __fastcall GFVXToAX(float x);                         // 0x009E1E10
long __fastcall GetNumberOfCharactersOfTypeInString(const CWideString& text, wchar_t character);   // 0x0048DBF9

class CPlayerGuiDef { public:
    unsigned char _pad000[0x5a4];
    C2DVector TextBoxTextAreaTLPos;                          // +0x5A4 (Ego_r +0x5E0)
    C2DVector TextBoxTextAreaBRPos;                          // +0x5AC (Ego_r +0x5E8)
};
class CPlayerGui { public:
    static const CPlayerGuiDef& __fastcall PeekGuiDef();     // 0x00438570
};

class CWStringParser : public CBaseClass { public:
    CWStringParser(const wchar_t* text);                     // 0x00437260
    CWideString ReadAsStringUntilCharacter(wchar_t character);   // 0x00A0CCD0
    void SkipWhitespace();                                   // 0x00A0C910
    const wchar_t* String;
    unsigned long StringPos;
    unsigned long Line;
};

class CSubtitleRenderer : public CSubtitleRendererLayout {
public:
    class CEmbeddedHighlight { public:
        CWideString Text;                                    // +0x00
        long        CharIndex;                               // +0x04
        C2DVector   Offset;                                  // +0x08
        bool        Split;                                   // +0x10
    };
    std::vector<CEmbeddedHighlight>& Highlights() { return *(std::vector<CEmbeddedHighlight>*)EmbeddedHighlights; }
    std::vector<CWideString>& ActionNames() { return *(std::vector<CWideString>*)EmbeddedHighlightActionNames; }
    CWideString& GetText() { return *(CWideString*)&Text_PStringData; }
    CFontBank* GetFont() const { return PFont; }
    float GetBlackBarHeight() const;                         // 0x0048DBC3
    void FormatTextForVWindow(const C2DBoxF& box, const CFontBank& font, const CWideString& text, CWideString& outText,
                              C2DBoxF& outBox) const;        // 0x0048F50C
    void ProcessEmbeddedHighlightTags();
};

void CSubtitleRenderer::ProcessEmbeddedHighlightTags()
{
    Highlights().clear();
    CWideString& original = GetText();
    CWideString text(original);
    long start = text.FindCharPos(L'<');
    while (start != -1) {
        long end = text.FindCharPos(L'>');
        if (end == -1)
            break;
        {
        CWideString tag = text.GetSubString(start + 1, end);
        if (!(tag != L""))
            break;
        CWideString prefix;
        if (start > 0)
            prefix = text.GetSubString(0, start);
        CWideString suffix;
        if (end < text.GetLength())
            suffix = text.GetSubString(end + 1, text.GetLength());
        text = prefix + L"'" + tag + L"'" + suffix;
        ActionNames().push_back(L"'" + tag + L"'");
        }
        start = text.FindCharPos(L'<');
    }
    for (unsigned int i = 0; i < ActionNames().size(); ++i) {
        CWideString name(ActionNames()[i]);
        if (name != L"") {
            long from = 0;
            for (unsigned int j = 0; j < Highlights().size(); ++j) {
                if (Highlights()[j].Text == name)
                    from = name.GetLength() + Highlights()[j].CharIndex;
            }
            CWideString rest = text.GetSubString(from, text.GetLength());
            const wchar_t* found = wcsstr(rest.GetAsCharArray(), name.GetAsCharArray());
            if (!found)
                break;
            long index = (long)(found - rest.GetAsCharArray()) + from;
            CEmbeddedHighlight highlight;
            highlight.Text = name;
            highlight.CharIndex = index;
            highlight.Split = false;
            Highlights().push_back(highlight);
        }
    }
    if (!Highlights().empty()) {
        const CPlayerGuiDef& def = CPlayerGui::PeekGuiDef();
        CWideString formatted;
        float width = (float)GPDisplayManager->GetRenderTargetDimensions().X;
        C2DBoxF box(GFAXToVX(def.TextBoxTextAreaTLPos.X),
                    (float)(g_screenHeight - GFAYToVY(GetBlackBarHeight() - def.TextBoxTextAreaTLPos.Y)),
                    GFAXToVX(width + def.TextBoxTextAreaBRPos.X - 640.0f),
                    (float)(g_screenHeight - GFAYToVY(GetBlackBarHeight() - def.TextBoxTextAreaBRPos.Y)));
        C2DBoxF formattedBox;
        FormatTextForVWindow(box, *PFont, text, formatted, formattedBox);
        std::vector<CEmbeddedHighlight> splits;
        for (unsigned int j = 0; j < Highlights().size(); ++j) {
            CWideString before = formatted.GetSubString(0, Highlights()[j].CharIndex);
            long pos = GetNumberOfCharactersOfTypeInString(before, L'\n');
            pos += Highlights()[j].CharIndex;
            CWideString word = formatted.GetSubString(pos, pos + Highlights()[j].Text.GetLength());
            long lineBreak = word.FindCharPos(L'\n');
            if (lineBreak > -1) {
                word = formatted.GetSubString(pos, Highlights()[j].Text.GetLength() + pos + 1);
                CWideString first = word.GetSubString(0, lineBreak);
                CWideString second = word.GetSubString(lineBreak + 1, word.GetLength());
                Highlights()[j].Text = first;
                CEmbeddedHighlight split;
                split.Text = second;
                split.CharIndex = Highlights()[j].CharIndex + lineBreak + 1;
                split.Split = true;
                splits.push_back(split);
            }
        }
        for (unsigned int k = 0; k < splits.size(); ++k)
            Highlights().push_back(splits[k]);
        for (unsigned int m = 0; m < Highlights().size(); ++m) {
            CWideString before = formatted.GetSubString(0, Highlights()[m].CharIndex);
            long lastLineBreaks = 0;
            for (;;) {
                long lineBreaks = GetNumberOfCharactersOfTypeInString(before, L'\n');
                if (lineBreaks == lastLineBreaks)
                    break;
                Highlights()[m].CharIndex += lineBreaks;
                before = formatted.GetSubString(0, Highlights()[m].CharIndex);
                lastLineBreaks = lineBreaks;
            }
            if (before.ToCharString().StartsWith("(HC)"))
                before = before.GetSubString(4, before.GetLength());
            C2DExtentsF extents = PFont->GetStringExtents(before, false);
            CWideString lastLine(before);
            CWStringParser parser(before);
            CWideString line = parser.ReadAsStringUntilCharacter(L'\n');
            while (line != L"") {
                lastLine = line;
                parser.SkipWhitespace();
                line = parser.ReadAsStringUntilCharacter(L'\n');
            }
            C2DExtentsF lastExtents = PFont->GetStringExtents(lastLine, false);
            if (Highlights()[m].Split)
                lastExtents.X = 0.0f;
            Highlights()[m].Offset = C2DVector(lastExtents.X, lastExtents.Y);
            Highlights()[m].Offset.Y -= (float)++lastLineBreaks;
            if (Highlights()[m].Offset.Y > 0.0f)
                Highlights()[m].Offset.Y -= PFont->GetFontMaxHeight();
            if (!g_FableLeftAlignText_013B861B) {
                CWideString rest = formatted.GetSubString(Highlights()[m].CharIndex, formatted.GetLength());
                CWStringParser restParser(rest);
                CWideString word = restParser.ReadAsStringUntilCharacter(L'\n');
                if (word == L"")
                    word = rest;
                CWideString wholeLine = lastLine + word;
                float boxWidth = GFVXToAX(formattedBox.BRX - formattedBox.TLX);
                Highlights()[m].Offset.X += (boxWidth - PFont->GetStringWidth(wholeLine)) * 0.5f;
            }
        }
    }
    original = text;
}
