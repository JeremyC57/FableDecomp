// NUISystem::CIconText::FitInWindow @ 0x00541A00
// Flags: /O2 /Oy
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