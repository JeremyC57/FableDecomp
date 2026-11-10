// GFStringToInt -- convert decimal text with a sign marker.
// Leaf: ?GFStringToInt@@YIJPBD@Z
// Retail: 0x009e1960, 74 bytes, fastcall.
#ifdef _MSC_VER
#define FABLE_FASTCALL __fastcall
#define FABLE_CDECL __cdecl
#else
#define FABLE_FASTCALL
#define FABLE_CDECL
#endif
#include <string.h>
long FABLE_FASTCALL GFStringToInt(const char* string)
{
    long top = 0;
    bool is_negative = false;
    long n = 0;
    long size = strlen(string);
    unsigned long c1 = 0;
    char ch = *(c1 + string);
    for (; c1 < (unsigned long)size; ch = *(++c1 + string))
    {
        if (ch == '-') is_negative = true;
        else { top *= 10; top += ch - '0'; }
    }
    n = top;
    if (is_negative == true) n *= -1;
    return n;
}