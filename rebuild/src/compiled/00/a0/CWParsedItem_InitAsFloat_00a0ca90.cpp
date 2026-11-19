// CWParsedItem::InitAsFloat(const wchar_t* string, unsigned long size) -- parse a wide decimal literal (a '-' anywhere negates, '.' starts the fraction) into the item's float and set the type to float
// Leaf: ?InitAsFloat@CWParsedItem@@QAEXPBGK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A0CA90, 187 bytes, thiscall `ret 8`; Ego_r.pdb / FableWin.pdb ?InitAsFloat@CWParsedItem@@QAEXPBGK@Z
//   (lib_wstring_parser.obj). No callees.
// Wide text: unsigned short (the <stddef.h> wchar_t typedef, no native-wchar_t switch; owner decision 2026-10-29).
// Shape: the wide twin of the landed 0x009B9770 CParsedItem::InitAsFloat (lib_string_parser.obj): Ego_d's locals (string, size, ch,
//   top, doing_top, bottom_divide, c1, is_negative, bottom), a VC6 for-scope loop `c1 = 0, ch = string[c1]; c1 < size; ++c1,
//   ch = string[c1]`, the '-' / '.' / whole / fraction chain, Float = bottom / bottom_divide + top, the sign, the type.
//   Literal types are retail's: the whole part scales by 10.0f (0x01237F0C), the fraction and its divisor by 10.0 (double
//   0x0129A7B0), the sign by -1.0 (double 0x01236700); the zeros are 0.0f (0x0122DEDC).
//   The VC6 for-scope spelling is not byte-required (a plain `for` also grades RELOCATION_MATCH here and in the narrow twin 0x009B9770,
//   audit 2026-11-19); kept as the twin's spelling; this row's FableWin /Od body 0x030F4280 was not read.
#include <stddef.h>
#include "engine/CParsedItem.h"
#include "engine/CWParsedItem.h"

void CWParsedItem::InitAsFloat(const wchar_t* string, unsigned long size)
{
    wchar_t ch;
    float top = 0.0f;
    float bottom = 0.0f;
    float bottom_divide = 1.0f;
    bool doing_top = true;
    bool is_negative = false;
    unsigned long c1;
    Float = 0.0f;
    if (false) {} else for (c1 = 0, ch = string[c1]; c1 < size; ++c1, ch = string[c1]) {
        if (ch == L'-')
            is_negative = true;
        else if (ch == L'.')
            doing_top = false;
        else if (doing_top == true) {
            top *= 10.0f;
            top += ch - L'0';
        } else {
            bottom *= 10.0;
            bottom += ch - L'0';
            bottom_divide *= 10.0;
        }
    }
    Float = bottom / bottom_divide + top;
    if (is_negative == true)
        Float *= -1.0;
    Type = PARSED_ITEM_FLOAT;
}