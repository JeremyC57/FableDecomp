// Behaviour test for 0x0071A7F0 (NF388 literal audit): links the row object and asserts the string the row hands to
// CCharString::CCharString(char const*, int): "CTCMapwho", length -1, one call, into the hidden return slot.
#include <stdio.h>
#include <string.h>
// CTCMapwho::GetClassTypeName @ 0071a7f0
class CCharString { public:
    char* storage_;
    CCharString(const char* text, long length);
};

class CTCMapwho { public:
    int dummy;
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
        CCharString r = ((CTCMapwho*)g_storage)->CTCMapwho::GetClassTypeName();
        where = &r;
        if (g_calls != 1 || g_len != -1 || g_text == 0 || strcmp(g_text, "CTCMapwho") != 0 || g_slot != where) {
            printf("FAIL_0071a7f0 calls=%d len=%ld text=%s\n", g_calls, g_len, g_text ? g_text : "(null)");
            return 1;
        }
    }
    printf("0071a7f0_TEST PASS\n");
    return 0;
}
