#include <stdio.h>
#include <assert.h>

// Begin exact selected source.
// GFStringToInt -- convert decimal text with a sign marker.
// Leaf: ?GFStringToInt@@YIJPBD@Z
// Retail: 0x009e1960, 74 bytes, fastcall.
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#endif
#include <string.h>
long __fastcall GFStringToInt(const char* string)
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
// End exact selected source.
int main()
{
    const char alphabet[4] = {'0','1','9','-'};
    char text[16];
    unsigned int combinations = 1;
    for (unsigned int length = 0; length <= 6; ++length)
    {
        for (unsigned int code = 0; code < combinations; ++code)
        {
            unsigned int digits = code;
            long expected = 0;
            bool minus = false;
            for (unsigned int i = 0; i < length; ++i)
            {
                text[i] = alphabet[digits % 4]; digits /= 4;
                if (text[i] == '-') minus = true;
                else expected = expected * 10 + text[i] - '0';
            }
            text[length] = 0;
            if (minus) expected = -expected;
            assert(GFStringToInt(text) == expected);
        }
        combinations *= 4;
    }
    char punctuation[4] = {'A',' ','2',0};
    assert(GFStringToInt(punctuation) == 1542);
    printf("009e1960_TEST PASS\n");
    return 0;
}