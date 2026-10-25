// CPlayerGui::FormatStringForArea(const C2DBoxF& area, const CFontBank& font, const CWideString& text, CWideString& out) -- word-wrap text into the area's width with the font: append each word and a space while the line fits, else start a new line with the word
// Grade: DIFFER(572v583) -- register allocation: ours enregisters the running line width in edi (spilling `area` to the frame and using a separate fild temp), retail keeps the width in its frame word [esp+10h] (stored 0 at entry, fild straight from it) and `area` in edi; every call, constant and branch agrees.
// Leaf: ?FormatStringForArea@CPlayerGui@@
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00437D10, 583 bytes, `ret 8`: static /Gr __fastcall (area ecx, font edx, text and out on the
// stack) as the landed caller CPlayerGui::AddScreenMessage 0x00438AA0 declares it; `and esp,-8` frame (8-byte C2DExtentsF locals).
// Callees: CWideString::operator const wchar_t* 0x0099AF00, CBaseClass() 0x0099A2D0 (CWStringParser's base; vtable 0x01231718
// stored inline), CWStringParser::Init 0x00A0C8E0 / ReadAsStringUntilWhitespace 0x00A0CD40 / SkipWhitespace 0x00A0C910,
// CWideString() 0x0099AED0 (the parsed item's string), the item's string getter 0x00A0CB80 (catalog "GetAnimFilter" is
// propagated), the font's GetStringExtents (slot 1) on GetAsCharArray 0x0099AF10, GFVXToAX 0x009E1DA0 / GFVYToAY 0x009E1DD0
// (converted in place), __ftol2 0x00BFEA70, CWideString operator+= (CWideString 0x0099B8D0, wchar_t* 0x0099B940), CWideString
// (const wchar_t*) 0x0099B6B0 for L" " 0x0122D820, operator+(const CWideString&, const wchar_t*) 0x0099BF30, L"\n" 0x01231728,
// ~CWideString 0x0099B510, ~CBaseClass 0x0099A2E0.
// Tried: a plain body (564v583: also fetched the word after the extents call's `push 0`); an inline extents helper returning
// by value (614v583, extra struct copies); the named `const CWideString& word` + newWidth form (572v583, this file); routing
// the width compares through a const-reference helper (identical 572v583) -- the width never leaves edi.
#include <stddef.h>
typedef unsigned short wchar16;          // wchar_t is not a built-in type under the retail flags

class CWideString {
public:
    void* PStringData;
    CWideString();                                      // 0x0099AED0
    CWideString(const wchar16* text);                   // 0x0099B6B0
    ~CWideString();                                     // 0x0099B510
    operator const wchar16*() const;                    // 0x0099AF00
    const wchar16* GetAsCharArray() const;              // 0x0099AF10
    CWideString& operator+=(const CWideString& other);  // 0x0099B8D0
    CWideString& operator+=(const wchar16* text);       // 0x0099B940
};
CWideString operator+(const CWideString& lhs, const wchar16* rhs);   // 0x0099BF30

struct C2DBoxF {
    float X0, Y0, X1, Y1;
    float GetWidth() const { return X1 - X0; }
};
struct C2DExtentsF {
    float Width;
    float Height;
};
class CFontBank {
public:
    virtual void v00();
    virtual C2DExtentsF GetStringExtents(const wchar16* text, bool wrap) const;   // slot 1 (+4)
};
float GFVXToAX(float x);                                // 0x009E1DA0
float GFVYToAY(float y);                                // 0x009E1DD0

class CBaseClass {
public:
    CBaseClass();                                       // 0x0099A2D0
    virtual ~CBaseClass();                              // 0x0099A2E0
};
struct CWParsedItem {                                   // 12 bytes
    long        Type;                                   // +0x00
    long        Integer;                                // +0x04
    CWideString String;                                 // +0x08
    CWideString& GetString();                           // 0x00A0CB80
};
class CWStringParser : public CBaseClass {              // vtable 0x01231718, 16 bytes
public:
    wchar16*      String;                               // +0x04
    unsigned long StringPos;                            // +0x08
    unsigned long Line;                                 // +0x0C
    CWStringParser(const wchar16* text) { Init(text); }
    void Init(const wchar16* text);                     // 0x00A0C8E0
    bool ReadAsStringUntilWhitespace(CWParsedItem* item);   // 0x00A0CD40
    void SkipWhitespace();                              // 0x00A0C910
};

class CPlayerGui {
public:
    static void FormatStringForArea(const C2DBoxF& area, const CFontBank& font, const CWideString& text, CWideString& out);
};

void CPlayerGui::FormatStringForArea(const C2DBoxF& area, const CFontBank& font, const CWideString& text, CWideString& out)
{
    long lineWidth = 0;
    CWStringParser parser(text);
    CWParsedItem item;
    while (parser.ReadAsStringUntilWhitespace(&item)) {
        const CWideString& word = item.GetString();
        C2DExtentsF wordExtents = font.GetStringExtents(word.GetAsCharArray(), false);
        wordExtents.Width = GFVXToAX(wordExtents.Width);
        wordExtents.Height = GFVYToAY(wordExtents.Height);
        long wordWidth = (long)wordExtents.Width;
        lineWidth += wordWidth;
        if (lineWidth < area.GetWidth()) {
            out += item.GetString();
            long spaceWidth;
            {
                CWideString space((const wchar16*)L" ");
                C2DExtentsF spaceExtents = font.GetStringExtents(space.GetAsCharArray(), false);
                spaceExtents.Width = GFVXToAX(spaceExtents.Width);
                spaceExtents.Height = GFVYToAY(spaceExtents.Height);
                spaceWidth = (long)spaceExtents.Width;
            }
            long newWidth = lineWidth + spaceWidth;
            if (newWidth < area.GetWidth()) {
                out += (const wchar16*)L" ";
                lineWidth = newWidth;
            }
            else {
                out += (const wchar16*)L"\n";
                lineWidth = 0;
            }
        }
        else {
            out += (const wchar16*)L"\n";
            {
                CWideString space((const wchar16*)L" ");
                C2DExtentsF spaceExtents = font.GetStringExtents(space.GetAsCharArray(), false);
                spaceExtents.Width = GFVXToAX(spaceExtents.Width);
                spaceExtents.Height = GFVYToAY(spaceExtents.Height);
                lineWidth = (long)(wordWidth + spaceExtents.Width);
            }
            out += item.GetString() + (const wchar16*)L" ";
        }
        parser.SkipWhitespace();
    }
}
