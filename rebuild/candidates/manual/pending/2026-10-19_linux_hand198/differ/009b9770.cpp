// Grade: DIFFER(177v181) -- divisor kept in memory only via `volatile` (probe artefact, not original); loop not rotated (text[i] load at top vs retail preload), bl/cl flag registers swapped, integer phase fmul+fiadd vs retail fild/fxch/fmul/faddp
// CParsedItem::InitAsFloat(const char*, unsigned long) -- parse a decimal literal (optional '-', digits, '.', digits) into the item's float and mark it PIT_FLOAT
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B9770, length 181 bytes.
// Callees: none. Float pool: 0.0f @ 0x0122DEDC (both accumulators), 10.0f @ 0x01237F0C (integer part),
//          10.0 (double) @ 0x0129A7B0 (fraction digits and their divisor), -1.0 (double) @ 0x01236700.
// Layout (PDB, matches the landed CStringParser rows): +0 EParsedItemType Type, +4 union { Integer; Float },
// +8 CCharString String.
// Shape: two float accumulators kept on the x87 stack for the whole loop -- the integer part is scaled by
// the float 10.0f, the fraction digits by the double 10.0 (with the divisor in a frame slot); a '-' anywhere
// marks the value negative and a '.' switches to the fraction. The flags are tested with `== true`
// (`cmp bl,1`), and the negation multiplies by the double -1.0.
enum EParsedItemType { PIT_NONE = 0, PIT_SYMBOL = 1, PIT_STRING = 2, PIT_INTEGER = 3, PIT_FLOAT = 4 };

struct CCharString { void* m_pData; };

struct CParsedItem {
    EParsedItemType Type;                          // +0x00
    union { long Integer; float Float; };          // +0x04
    CCharString     String;                        // +0x08
    void InitAsFloat(const char* text, unsigned long length);
};

void CParsedItem::InitAsFloat(const char* text, unsigned long length)
{
    float wholePart = 0.0f;
    float fractionPart = 0.0f;
    Integer = 0;
    volatile float divisor = 1.0f;
    bool negative = false;
    bool inWholePart = true;
    for (unsigned long i = 0; i < length; ++i) {
        char c = text[i];
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
    }
    Type = PIT_FLOAT;
    Float = wholePart + fractionPart / divisor;
    if (negative == true)
        Float = Float * -1.0;
}
