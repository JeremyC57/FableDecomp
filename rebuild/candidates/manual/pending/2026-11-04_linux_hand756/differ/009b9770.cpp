// CParsedItem::InitAsFloat(const char* text, unsigned long length) -- parse a decimal literal (a '-' anywhere negates, '.' starts the fraction) into the item's float and set the type to float
// Grade: DIFFER(ours v retail) -- DIFFER(171v181) (try o): retail keeps the divisor in a frame slot (`mov [esp+10h],3F800000h`, fld/fmul/fstp per fraction digit, `fdiv [esp+10h]`) with only the two accumulators on the x87 stack and the minus flag in bl; VC7.1 here enregisters all three floats (fld 1.0 at entry, fxch shuffles) and puts the minus flag in cl. Loop shape (pre-read char, entry test, do/while) now matches. The wide twin CWParsedItem::InitAsFloat 0x00A0CA90 has the same bytes.
// Leaf: ?InitAsFloat@CParsedItem@@QAEXPBDK@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x009B9770, 181 bytes, `ret 8`; Ego_r.pdb ?InitAsFloat@CParsedItem@@QAEXPBDK@Z. No callees.
// Pools: 0.0f 0x0122DEDC, 10.0f 0x01237F0C (whole part), 10.0 double 0x0129A7B0 (fraction + divisor), -1.0 double 0x01236700.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CPARSEDITEM
#include "engine/CParsedItem.h"

void CParsedItem::InitAsFloat(const char* text, unsigned long length)
{
    float wholePart = 0.0f;
    float fractionPart = 0.0f;
    unsigned long i = 0;
    Integer = 0;
    float divisor = 1.0f;
    bool negative = false;
    bool inWholePart = true;
    char c = text[0];
    if (i < length) {
        do {
            if (c == '-')
                negative = true;
            else if (c == '.')
                inWholePart = false;
            else {
                long digit = c - '0';
                if (inWholePart == true)
                    wholePart = digit + wholePart * 10.0f;
                else {
                    fractionPart = fractionPart * 10.0 + digit;
                    divisor = divisor * 10.0;
                }
            }
            c = text[++i];
        } while (i < length);
    }
    Type = PARSED_ITEM_FLOAT;
    Float = wholePart + fractionPart / divisor;
    if (negative == true)
        Float = Float * -1.0;
}
