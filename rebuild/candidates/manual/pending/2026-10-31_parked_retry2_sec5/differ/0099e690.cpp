// GFCharStringToFloat(const CCharString& text) -- parse a decimal float: '-' anywhere makes it negative, '.' switches to the fraction, every other character counts as a digit
// Grade: DIFFER(176v180) -- true extent 180 (catalog 344 swallowed an uncatalogued function at 0x0099E750; BOUNDS_FIX +
//   oracle_extra); divisor declared first (47 diff lines v 56 at the true extent, v 134 before); left: retail homes the
//   divisor in a frame word, VC7.1 enregisters it
// Retail: Fable.exe (TLC, VC7.1) @ 0x0099E690, 180 bytes. FableWin.pdb ?GFCharStringToFloat@@YIMABVCCharString@@@Z:
// a free `__fastcall` (text in ecx) returning float in st(0), plain `ret`.
// Constants: 0.0f @ 0x0122DEDC, 10.0f @ 0x01237F0C, 10.0 (double) @ 0x0129A7B0. Literal "" @ 0x0129AAF4.
// Shape: the GFCharStringToInt (0x0099E7F0) fetch-ahead loop with an unsigned index; no call in the loop, so the block
// pointer stays in ecx. The whole part and the fraction live on the x87 stack, the divisor in a frame word: whole =
// whole * 10.0f + digit (retail: `fild` first, then fxch/fmul/faddp), fraction = fraction * 10.0 + digit (`fiadd`),
// divisor *= 10.0 (double multiply, float store); result = whole + fraction / divisor, negated on '-'.
// Layout (landed CCharString model): CCharStringData = CBasicString<char> {PData +0, StringLength +4, ...}.
// Leaf: ?GFCharStringToFloat@@
struct CBasicStringChar {
    char*         PData;                                // +0x00
    unsigned long StringLength;                         // +0x04
};
struct CCharStringData {
    CBasicStringChar Data;                              // +0x00
};

class CCharString
{
public:
    CCharStringData* PStringData;                       // +0x00
    const char* GetString() const { return PStringData == 0 ? "" : PStringData->Data.PData; }
    long GetLength() const { return PStringData == 0 ? 0 : PStringData->Data.StringLength; }
    char GetChar(long index) const { return GetString()[index]; }
};

float __fastcall GFCharStringToFloat(const CCharString& text)
{
    float divisor = 1.0f;
    float whole = 0.0f;
    float fraction = 0.0f;
    bool negative = false;
    unsigned long i = 0;
    bool beforePoint = true;
    char c = text.GetChar(i);
    while (i < (unsigned long)text.GetLength()) {
        if (c == '-')
            negative = true;
        else if (c == '.')
            beforePoint = false;
        else {
            long digit = c - '0';
            if (beforePoint == true)
                whole = whole * 10.0f + (float)digit;
            else {
                fraction = fraction * 10.0 + digit;
                divisor *= 10.0;
            }
        }
        c = text.GetChar(++i);
    }
    float result = whole + fraction / divisor;
    if (negative)
        result = -result;
    return result;
}
