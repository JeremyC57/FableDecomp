// Behaviour test for 0x00B66DC0 (NF388 literal audit): links the row object and asserts the string the row hands to
// CCharString::CCharString(char const*, int): "EnableSky", length -1, one call, into the hidden return slot.
#include <stdio.h>
#include <string.h>
// CEngineSkyRenderer::GetConsoleEnableFunctionName
// Retail 0x00b66dc0: constructs a CCharString from a string literal, returns by value.

class CCharString {
public:
    char* m_ptr;
    CCharString(const char* s, long len);
};

class CEngineSkyRenderer {
public:
    virtual CCharString GetConsoleEnableFunctionName();
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
        CCharString r = ((CEngineSkyRenderer*)g_storage)->CEngineSkyRenderer::GetConsoleEnableFunctionName();
        where = &r;
        if (g_calls != 1 || g_len != -1 || g_text == 0 || strcmp(g_text, "EnableSky") != 0 || g_slot != where) {
            printf("FAIL_00b66dc0 calls=%d len=%ld text=%s\n", g_calls, g_len, g_text ? g_text : "(null)");
            return 1;
        }
    }
    printf("PASS_SKYRENDER_NAME\n");
    return 0;
}
