// CWParsedItem::InitAsFloat(const wchar_t* text, unsigned long length) -- parses a decimal number (optional '-', digits, optional '.' and fraction digits) from a wide-character span into the item's float value and marks the item as a float (type 4)
// Grade: DIFFER(ours v retail) -- 172v187 on engine/CWParsedItem.h: retail keeps only the integer part and fraction on the x87 stack and homes the divisor in the push-ecx frame word (immediate 1.0f store, fld/fmul/fstp per digit, fdiv from memory) and converts the integer-part digit with fild before the multiply; VC7.1 here enregisters all three and folds the digit into fiadd; the two bool flags swap bl/cl. Tried here: digit + integerPart*10.0f (canonicalised, identical); 10 earlier grades on the local model (declaration order, compound ops, operand order, float digit local, explicit casts).
// Leaf: ?InitAsFloat@CWParsedItem@@QAEXPBGK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A0CA90, 187 bytes, __thiscall, `ret 8`. No callees.
#include <stddef.h>
#include "engine/CWParsedItem.h"
extern const float g_fZero;                        // 0x0122DEDC (0.0f)

void CWParsedItem::InitAsFloat(const wchar_t* text, unsigned long length)
{
    float divisor = 1.0f;
    float integerPart = g_fZero;
    float fraction = g_fZero;
    bool negative = false;
    bool beforePoint = true;
    unsigned long i = 0;
    Float = 0.0f;
    wchar_t c = *text;
    while (i < length) {
        if (c == L'-')
            negative = true;
        else if (c == L'.')
            beforePoint = false;
        else {
            long digit = c - L'0';
            if (beforePoint == true)
                integerPart = integerPart * 10.0f + digit;
            else {
                fraction = fraction * 10.0 + digit;
                divisor = divisor * 10.0;
            }
        }
        c = text[++i];
    }
    Type = (EParsedItemType)4;
    Float = integerPart + fraction / divisor;
    if (negative == true)
        Float = Float * -1.0;
}
