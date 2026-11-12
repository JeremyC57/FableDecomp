// CParsedItem::InitAsFloat(const char* text, unsigned long length) -- parse a decimal literal (a '-' anywhere negates, '.' starts the fraction) into the item's float and set the type to float
// Grade: DIFFER(171v181) -- regraded as it sits (chunk Q12, header overlay on), not reworked: local model of classes that have or need shared headers -- the migration is the next step. Earlier diagnosis: divisor kept in memory only via `volatile` (probe artefact, not original); loop not rotated (text[i] load at top vs retail preload), bl/cl flag registers swapped, integer phase fmul+fiadd vs retail fild/fxch/fmul/faddp CParsedItem::InitAsFloat(const char*, unsigned long) -- parse a decimal literal (optional '-', digits, '.', digits) into the item's float and mark it PIT_FLOAT
// Leaf: ?InitAsFloat@CParsedItem@@QAEXPBDK@Z
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
