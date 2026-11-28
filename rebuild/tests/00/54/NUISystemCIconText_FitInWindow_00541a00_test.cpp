// NUISystem::CIconText::FitInWindow @ 0x00541A00
// Leaf: ?FitInWindow@CIconText@NUISystem@@IAEJAAVCWideString@@ABVC2DBoxF@@@Z
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CFONTBANK_HEIGHT
#define FABLE_RETAIL_ICONTEXT_FONT_INLINE_PASS29
#define FABLE_RETAIL_CFONTBANK_ICONTEXT_FIT_PASS30
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_STLP_MAX_FASTCALL
#define FABLE_RETAIL_SUBTITLE_WRAP_BODY_PASS29
#define FABLE_RETAIL_CWSTRINGPARSER_ICONTEXT_PASS30
#include "engine/NUISystem_CIconText.h"
#include "engine/CFontBank.h"
#include "engine/CWStringParser.h"
#include "engine/CWParsedItem.h"
#include "engine/CWideStringOperators.h"
#include "fable_assert.h"

extern "C" int FABLE_CDECL iswspace(unsigned short);

long NUISystem::CIconText::FitInWindow(CWideString& text, const C2DBoxF& window)
{
    long result = PFont->GetFontHeight();
    if (window.GetWidth() <= 0.0f && window.GetHeight() <= 0.0f)
        return result;

    CWStringParser parser(text);
    CWideString out_string;
    CWParsedItem item;
    float window_x = 0.0f;
    float max_window_x = 0.0f;
    while (parser.ReadNextChunkAsString(&item)) {
        CWideString string_part(item.GetString());
        float string_length = PFont->GetStringWidth(string_part);
        bool is_space = iswspace(string_part.GetChar(0)) != 0;
        if (string_part.GetChar(0) == L'\n') {
            long char_index = 0;
            do {
                out_string += L"\n";
                ++char_index;
                result += PFont->GetFontHeight();
            } while (string_part.GetChar(char_index) == L'\n');
            window_x = 0.0f;
            continue;
        }
        if (string_part.GetChar(string_part.GetLength() - 1) == L'\n') {
            max_window_x = (std::max)(window_x, max_window_x);
            window_x = 0.0f;
            result += PFont->GetFontHeight();
        }
        if (!is_space && window_x + string_length >= window.GetWidth()
                      && out_string.GetChar(out_string.GetLength() - 1) != L'\n') {
            max_window_x = (std::max)(window_x, max_window_x);
            out_string += L"\n";
            window_x = 0.0f;
            result += PFont->GetFontHeight();
        }
        while (string_length >= window.GetWidth()) {
            window_x = 0.0f;
            for (long c1 = 0; c1 < string_part.GetLength(); ++c1) {
                CWideString ch = CWideString(L"") + string_part.GetChar(c1);
                float char_width = PFont->GetStringWidth(ch);
                if (window_x + char_width < window.GetWidth()) {
                    out_string += ch;
                    window_x += char_width;
                } else {
                    if (string_part.GetChar(c1) != L'\n' && c1 + 1 < string_part.GetLength()
                                                            && string_part.GetChar(c1 + 1) != L'\n')
                        out_string += L"\n";
                    if (c1 > string_part.GetLength()) {
                        FABLE_RETAIL_ASSERT(c1 <= string_part.GetLength());
                        break;
                    }
                    string_part = string_part.GetAsCharArray() + c1;
                    string_length = PFont->GetStringWidth(string_part);
                    max_window_x = (std::max)(window_x, max_window_x);
                    window_x = 0.0f;
                    result += PFont->GetFontHeight();
                    break;
                }
            }
        }
        out_string += string_part;
        window_x += string_length;
    }
    text = out_string;
    return result;
}


const CWideString& CWParsedItem::GetString() const
{
    return String;
}
void CWStringParser::Init(const wchar_t* text)
{
    String = text;
    StringPos = 0;
    Line = 0;
}
#include <assert.h>
#include <stdio.h>
#include <string.h>

extern "C" int FABLE_CDECL iswspace(unsigned short);

static wchar_t EmptyStorage[2] = {0, 0};

static long Length(const wchar_t* value)
{
    long size = 0;
    while (value[size])
        ++size;
    return size;
}

static wchar_t* CopySpan(const wchar_t* value, long size)
{
    wchar_t* const storage = new wchar_t[size+2];
    storage[0] = 0;
    memcpy(storage+1, value, size*sizeof(wchar_t));
    storage[size+1] = 0;
    return storage;
}

CWideString::CWideString() : PStringData(0) {}
CWideString::CWideString(const wchar_t* value) : PStringData(0) { *this = value; }
CWideString::CWideString(const CWideString& value) : PStringData(0) { *this = value; }
CWideString::~CWideString() { delete[] reinterpret_cast<wchar_t*>(PStringData); }
const wchar_t* CWideString::GetAsCharArray() const
{
    return PStringData ? reinterpret_cast<const wchar_t*>(PStringData)+1 : EmptyStorage+1;
}
CWideString::operator const wchar_t*() const { return GetAsCharArray(); }
long CWideString::GetLength() const { return Length(GetAsCharArray()); }
const CWideString& CWideString::operator=(const CWideString& value)
{
    return *this = value.GetAsCharArray();
}
const CWideString& CWideString::operator=(const wchar_t* value)
{
    wchar_t* const copy = CopySpan(value,Length(value));
    delete[] reinterpret_cast<wchar_t*>(PStringData);
    PStringData = reinterpret_cast<CWideStringData*>(copy);
    return *this;
}
const CWideString& CWideString::operator+=(const wchar_t* value)
{
    const long left = GetLength();
    const long right = Length(value);
    wchar_t* const merged = new wchar_t[left+right+2];
    merged[0] = 0;
    memcpy(merged+1, GetAsCharArray(), left*sizeof(wchar_t));
    memcpy(merged+1+left, value, (right+1)*sizeof(wchar_t));
    delete[] reinterpret_cast<wchar_t*>(PStringData);
    PStringData = reinterpret_cast<CWideStringData*>(merged);
    return *this;
}
const CWideString& CWideString::operator+=(const CWideString& value)
{
    return *this += value.GetAsCharArray();
}
CWideString FABLE_FASTCALL operator+(const CWideString& left, wchar_t right)
{
    CWideString copy(left);
    const wchar_t next[] = {right, 0};
    copy += next;
    return copy;
}

#ifndef LINK_ORIGINAL_PARSER
CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
#endif
CCharString::CCharString() : storage_(0) {}
CCharString::~CCharString() {}
CFontBank::~CFontBank() {}
bool CFontBank::Preload(const CWideString&, bool) const { assert(false); return false; }

struct IconTextNativeFixture : NUISystem::CIconText
{
    static bool Equals(const wchar_t* lhs, const wchar_t* rhs)
    {
        while (*lhs == *rhs) {
            if (!*lhs)
                return true;
            ++lhs;
            ++rhs;
        }
        return false;
    }
    static long Call(NUISystem::CIconText* text, CWideString& value, const C2DBoxF& window)
    {
        typedef long (NUISystem::CIconText::*Fit)(CWideString&, const C2DBoxF&);
        Fit fit = &IconTextNativeFixture::FitInWindow;
        return (text->*fit)(value, window);
    }
    static void SetParsedString(CWParsedItem* item, const wchar_t* string)
    {
        const_cast<CWideString&>(item->GetString()) = string;
    }
};

#ifdef LINK_ORIGINAL_PARSER
void CWideString::SetAsSubString(const wchar_t* value, long length)
{
    wchar_t* const copy = CopySpan(value,length);
    delete[] reinterpret_cast<wchar_t*>(PStringData);
    PStringData = reinterpret_cast<CWideStringData*>(copy);
}
#else
bool CWStringParser::ReadNextChunkAsString(CWParsedItem* item)
{
    if (!String[StringPos])
        return false;
    const unsigned long start = StringPos;
    const bool space = iswspace(String[start]) != 0;
    ++StringPos;
    while (String[StringPos] && ((iswspace(String[StringPos]) != 0) == space))
        ++StringPos;
    wchar_t* const copy = CopySpan(String+start, StringPos-start);
    IconTextNativeFixture::SetParsedString(item, copy+1);
    delete[] copy;
    return true;
}

#endif

class FixtureFont : public CFontBank
{
public:
    FixtureFont() : calls(0) { FontHeight = 10; }
    virtual C2DExtentsF GetStringExtents(const wchar_t* value, bool include_spaces) const
    {
        assert(!include_spaces);
        ++calls;
        float width = 0;
        while (*value) {
            if (*value != L'\n')
                width += *value == L'W' ? 2.0f : 1.0f;
            ++value;
        }
        return C2DExtentsF(width, 1.0f);
    }
    virtual float GetFontMaxHeight() const { assert(false); return 0; }
    virtual void PrepareToRender(bool) const { assert(false); }
    virtual void Render(const CWideString&, const C3DVector&, float, const CRGBColour&,
                        unsigned long, const CTexture*) const { assert(false); }
    mutable int calls;
};

static void Check(float width, float height, const wchar_t* before, const wchar_t* after,
                  long result, unsigned int minimum_measurements)
{
    FixtureFont font;
    union { unsigned long align; unsigned char bytes[sizeof(NUISystem::CIconText)]; } icon;
    memset(icon.bytes,0,sizeof(icon.bytes));
    CFontBank* const bank = &font;
    memcpy(icon.bytes+0x164,&bank,sizeof(bank));
    NUISystem::CIconText* receiver = reinterpret_cast<NUISystem::CIconText*>(icon.bytes);
    C2DBoxF window;
    window.TLX = 0;
    window.TLY = 0;
    window.BRX = width;
    window.BRY = height;
    CWideString text(before);
    assert(IconTextNativeFixture::Call(receiver,text,window) == result);
    assert(IconTextNativeFixture::Equals(text.GetAsCharArray(),after));
    assert(font.calls >= (int)minimum_measurements);
}

int main()
{
    FixtureFont helper_font;
    assert(helper_font.GetStringWidth(CWideString(L"ab W")) == 5.0f);
    assert(helper_font.calls == 1);
    Check(0, 0, L"ab cd", L"ab cd", 10, 0);
    Check(9, 5, L"", L"", 10, 0);
    Check(9, 5, L"ab cd", L"ab cd", 10, 3);
    Check(5, 5, L"ab cd", L"ab \ncd", 20, 3);
    Check(4, 5, L"Q abcdef", L"Q \nabc\ndef", 30, 8);
    Check(9, 5, L"ab\n\ncd", L"ab\n\ncd", 30, 3);
    Check(6, 5, L"ab WW", L"ab \nWW", 20, 3);
    Check(9, 0, L"x", L"x", 10, 1);
    Check(-1, 0, L"x", L"x", 10, 0);
    Check(9, 5, L"ab \ncd", L"ab \ncd", 20, 3);
    Check(9, 5, L"\nq", L"\nq", 20, 2);
    puts("00541a00_TEST PASS");
    return 0;
}