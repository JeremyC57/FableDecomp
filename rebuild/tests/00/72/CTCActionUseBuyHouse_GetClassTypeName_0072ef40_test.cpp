// Behaviour test for 0x0072EF40 (NF388 literal audit): links the row object and asserts the string the row hands to
// CCharString::CCharString(char const*, int): "CTCActionUseBuyHouse", length -1, one call, into the hidden return slot.
#include <stdio.h>
#include <string.h>
// CTCActionUseBuyHouse::GetClassTypeName @ 0072ef40
// __fastcall returning CCharString by value; ignores `this`, builds string from a literal.

class CCharString {
public:
    CCharString(const char* s, long len);
    void* m_data;
};

// The ctor is external (relocation-masked). Provide a declaration only; the
// harness links a real impl for the test but masks its address in the compare.
class CTCActionUseBuyHouse {
public:
    CCharString GetClassTypeName();
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
        CCharString r = ((CTCActionUseBuyHouse*)g_storage)->CTCActionUseBuyHouse::GetClassTypeName();
        where = &r;
        if (g_calls != 1 || g_len != -1 || g_text == 0 || strcmp(g_text, "CTCActionUseBuyHouse") != 0 || g_slot != where) {
            printf("FAIL_0072ef40 calls=%d len=%ld text=%s\n", g_calls, g_len, g_text ? g_text : "(null)");
            return 1;
        }
    }
    printf("0072ef40_TEST PASS\n");
    return 0;
}
