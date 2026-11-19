// CParsedItem::InitAsFloat(const char* string, unsigned long size) -- parse a decimal literal (a '-' anywhere negates, '.' starts the fraction) into the item's float and set the type to float
// Leaf: ?InitAsFloat@CParsedItem@@QAEXPBDK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009B9770, 181 bytes, thiscall `ret 8`; Ego_r.pdb / FableWin.pdb ?InitAsFloat@CParsedItem@@QAEXPBDK@Z
//   (lib_string_parser.obj). No callees.
// Shape: FableWin 0x030595B0 (/Od) statement order with Ego_d's locals (string, size, ch, top, doing_top, bottom_divide, c1, is_negative,
//   bottom): the three accumulators and two flags initialised, Float cleared, a VC6 for-scope loop `c1 = 0, ch = string[c1]; c1 < size;
//   ++c1, ch = string[c1]`, the '-' / '.' / whole / fraction chain, Float = bottom / bottom_divide + top, the sign, the type.
//   Literal types are retail's (FableWin's are all double): the whole part scales by 10.0f (0x01237F0C), the fraction and its divisor by
//   10.0 (double 0x0129A7B0), the sign by -1.0 (double 0x01236700); the zeros are 0.0f (0x0122DEDC).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CPARSEDITEM
#include "engine/CParsedItem.h"

void CParsedItem::InitAsFloat(const char* string, unsigned long size)
{
    char ch;
    float top = 0.0f;
    float bottom = 0.0f;
    float bottom_divide = 1.0f;
    bool doing_top = true;
    bool is_negative = false;
    unsigned long c1;
    Float = 0.0f;
    if (false) {} else for (c1 = 0, ch = string[c1]; c1 < size; ++c1, ch = string[c1]) {
        if (ch == '-')
            is_negative = true;
        else if (ch == '.')
            doing_top = false;
        else if (doing_top == true) {
            top *= 10.0f;
            top += ch - '0';
        } else {
            bottom *= 10.0;
            bottom += ch - '0';
            bottom_divide *= 10.0;
        }
    }
    Float = bottom / bottom_divide + top;
    if (is_negative == true)
        Float *= -1.0;
    Type = PARSED_ITEM_FLOAT;
}