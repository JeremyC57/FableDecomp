// GFStringToFloat(const wchar_t*) -- wide-string overload: digits before the first '.' build the integer part, later digits the fraction (divisor x10 each), any '-' negates the result
// Leaf: ?GFStringToFloat@@YIMPBG@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E19B0, length 198 bytes (+ int3 padding).
// PDB (FableWin): ?GFStringToFloat@@YIMPB_W@Z -- __fastcall, string in ecx, float result. The catalog label
// "GFParseFloat" is descriptive; the PDB pairs this body with the char overload at 0x009E18B0 (same source shape).
// Callee: MSVCR71 wcslen (0x00BFED94). Constants as in the char overload (0.0f/1.0f/-1.0f, 10.0f, double 10.0).
// Shape: the three accumulators are initialised in frame slots because the wcslen call sits between their
// initialisation and the loop (the x87 stack is not preserved across calls), then loaded onto the FPU for the loop;
// the current character is read before the loop and re-read at the bottom (`mov cx,[esi+edx*2+2]`).
// The tail is `fraction /= divisor; result = fraction + integer;`: the wide overload's `fxch st(1)` before the final
// `faddp` only appears with the separate divide statement (`integer + fraction / divisor` drops it); both overloads
// match with this one source form.
typedef unsigned short wchar_t;
extern "C" unsigned int __cdecl wcslen(const wchar_t* text);   // 0x00BFED94

float __fastcall GFStringToFloat(const wchar_t* text)
{
    float integer = 0.0f;
    float fraction = 0.0f;
    float divisor = 1.0f;
    bool inInteger = true;
    bool negative = false;
    unsigned long length = wcslen(text);
    unsigned long i = 0;
    wchar_t c = *text;
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