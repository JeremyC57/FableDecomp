// CWParsedItem::InitAsFloat(const wchar_t* text, unsigned long length) -- parses a decimal number (optional '-', digits, optional '.' and fraction digits) from a wide-character span into the item's float value and marks the item as a float (type 4)
// Grade: DIFFER(172v187) -- x87 allocation: retail keeps the integer part and fraction on the FPU and the divisor in the push-ecx frame word (immediate 1.0f store, fld/fmul/fstp per digit, fdiv from memory), and converts the integer-part digit with fild before the multiply; VC7.1 here enregisters all three and folds the digit into fiadd; 10 grades (declaration order, compound ops, operand order, float digit local, explicit casts)
// Leaf: ?InitAsFloat@CWParsedItem@@QAEXPB_WK@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A0CA90, 187 bytes, `ret 8`.
//   PDB name ?InitAsFloat@CWParsedItem@@QAEXPB_WK@Z.
// Callees: none. Constants: 0.0f 0x0122DEDC (an extern float, loaded twice), 10.0f 0x01237F0C, 10.0 (double)
//   0x0129A7B0, -1.0 (double) 0x01236700.
// Shape: the integer part and the fraction ride on the x87 stack for the whole loop; the integer part uses the float
//   literal 10.0f, the fraction and its divisor the double literal 10.0; the digit goes through the dead `text`
//   argument slot for `fild`; the character is read ahead (`c = text[++i]` at the loop end, so the span's terminator
//   is read once; a `for` over text[i] gets unrolled by four); the two flags are bools compared with `== true` (`cmp bl,1`).
// Layout (Ego_r.pdb): Type +0 (EParsedItemType), value union +4, CWideString String +8.
typedef unsigned short wchar16;                    // wchar_t under the era defaults
extern const float g_fZero;                        // 0x0122DEDC (0.0f)
class CWideString { public: void* PStringData; };
class CWParsedItem { public:
    long Type;                                     // +0x00 EParsedItemType (4 = float)
    union {
        long  Integer;
        float Float;
    } Value;                                       // +0x04
    CWideString String;                            // +0x08
    void InitAsFloat(const wchar16* text, unsigned long length);
};
void CWParsedItem::InitAsFloat(const wchar16* text, unsigned long length)
{
    float divisor = 1.0f;
    float integerPart = g_fZero;
    float fraction = g_fZero;
    bool negative = false;
    bool beforePoint = true;
    unsigned long i = 0;
    Value.Float = 0.0f;
    wchar16 c = *text;
    while (i < length) {
        if (c == '-')
            negative = true;
        else if (c == '.')
            beforePoint = false;
        else {
            long digit = c - '0';
            if (beforePoint == true)
                integerPart = integerPart * 10.0f + digit;
            else {
                fraction = fraction * 10.0 + digit;
                divisor = divisor * 10.0;
            }
        }
        c = text[++i];
    }
    Type = 4;
    Value.Float = integerPart + fraction / divisor;
    if (negative == true)
        Value.Float = Value.Float * -1.0;
}
