// GFStringToFloat(const char*) -- parse a decimal number: digits before the first '.' build the integer part, later digits the fraction (divisor x10 each), any '-' negates the result
// Leaf: ?GFStringToFloat@@YIMPBD@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E18B0, length 176 bytes.
// PDB (FableWin): ?GFStringToFloat@@YIMPBD@Z -- __fastcall, string in ecx, float result on the FPU.
// Constants: 0.0f 0x0122DEDC, 1.0f 0x0122DED8, -1.0f 0x0122DEE0, 10.0f 0x01237F0C (the per-digit scale) and the
// DOUBLE 10.0 at 0x0129A7B0 for the divisor (`fmul qword`), so the divisor is scaled by a double literal.
// Shape: the three accumulators live on the FPU stack for the whole loop (integer, fraction, divisor); strlen is the
// /Oi intrinsic; the current character is read before the loop and re-read at the bottom (`mov dl,[esi+ecx+1]`),
// the explicit hoisted form (see 0x009E1960); the `inInteger` flag sits in bl and `negative` in a frame byte.
// The tail is `fraction /= divisor; result = fraction + integer;`: the wide overload's `fxch st(1)` before the final
// `faddp` only appears with the separate divide statement (`integer + fraction / divisor` drops it); both overloads
// match with this one source form.
#include <string.h>

float __fastcall GFStringToFloat(const char* text)
{
    float integer = 0.0f;
    float fraction = 0.0f;
    float divisor = 1.0f;
    bool inInteger = true;
    bool negative = false;
    unsigned long length = strlen(text);
    unsigned long i = 0;
    char c = *text;
    while (i < length)
    {
        if (c == '-')
            negative = true;
        else if (c == '.')
            inInteger = false;
        else if (inInteger == true)
            integer = integer * 10.0f + (c - '0');
        else
        {
            fraction = fraction * 10.0f + (c - '0');
            divisor *= 10.0;
        }
        c = *(++i + text);
    }
    fraction /= divisor;
    float result = fraction + integer;
    if (negative == true)
        result *= -1.0f;
    return result;
}