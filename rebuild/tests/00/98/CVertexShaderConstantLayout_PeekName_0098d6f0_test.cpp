// Behaviour test for 0x0098D6F0 (NF388 literal audit): links the row object and asserts the string the row hands to
// CCharString::CCharString(char const*, int): "", length -1, one call, into the hidden return slot.
#include <stdio.h>
#include <string.h>
// CVertexShaderConstantLayout::  @ 0098d6f0
// __fastcall, returns CCharString by value from a string literal.

class CCharString {
public:
    CCharString(const char* s, long count);
private:
    char* m_data;
};

class CVertexShaderConstantLayout {
public:
    CCharString PeekName();
};


static const char* g_text = 0;
static long g_len = 0;
static int g_calls = 0;
static const void* g_slot = 0;
static char g_storage[1024];
CCharString::CCharString(const char* s, long n) { g_text = s; g_len = n; ++g_calls; g_slot = this; }
int main(void)
{
    const void* where = 0;
    {
        CCharString r = ((CVertexShaderConstantLayout*)g_storage)->CVertexShaderConstantLayout::PeekName();
        where = &r;
        if (g_calls != 1 || g_len != -1 || g_text == 0 || strcmp(g_text, "") != 0 || g_slot != where) {
            printf("FAIL_0098d6f0 calls=%d len=%ld text=%s\n", g_calls, g_len, g_text ? g_text : "(null)");
            return 1;
        }
    }
    printf("PEEKNAME_OK\n");
    return 0;
}
