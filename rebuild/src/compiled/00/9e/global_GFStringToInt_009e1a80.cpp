// GFStringToInt -- convert decimal text with a sign marker.
// Leaf: ?GFStringToInt@@YIJPBG@Z
// Retail: 0x009e1a80, 75 bytes, fastcall.
#ifdef _MSC_VER
#define FABLE_FASTCALL __fastcall
#define FABLE_CDECL __cdecl
#else
#define FABLE_FASTCALL
#define FABLE_CDECL
#endif
extern "C" unsigned int FABLE_CDECL wcslen(const unsigned short*);
long FABLE_FASTCALL GFStringToInt(const unsigned short* string)
{
    long top = 0;
    bool is_negative = false;
    long n = 0;
    long size = wcslen(string);
    unsigned long c1 = 0;
    unsigned short ch = *(c1 + string);
    for (; c1 < (unsigned long)size; ch = *(++c1 + string))
    {
        if (ch == '-') is_negative = true;
        else { top *= 10; top += ch - '0'; }
    }
    n = top;
    if (is_negative == true) n *= -1;
    return n;
}