// Behaviour test for 0x00B88F80 (NF388 literal audit): links the row object and asserts the string the row hands to
// CCharString::CCharString(char const*, int): "EnableScreenEffectDisplacementRenderer", length -1, one call, into the hidden return slot.
#include <stdio.h>
#include <string.h>
#include "fable_string.h"

// CEngineScreenEffectDisplacementRenderer is a screen-effect renderer whose
// console-registration virtuals return the fixed script-console function name.
// The retail leaf ignores its 'this' pointer and simply materialises the
// CCharString return slot from a fixed string literal with length -1
// (auto-measure).
class CEngineScreenEffectDisplacementRenderer
{
public:
    virtual CCharString GetConsoleEnableFunctionName();
};


static const char* g_text = 0;
static long g_len = 0;
static int g_calls = 0;
static const void* g_slot = 0;
static char g_storage[1024];
CCharString::CCharString(const char* s, long n) { g_text = s; g_len = n; ++g_calls; g_slot = this; }
CCharString::~CCharString() {}
int main(void)
{
    const void* where = 0;
    {
        CCharString r = ((CEngineScreenEffectDisplacementRenderer*)g_storage)->CEngineScreenEffectDisplacementRenderer::GetConsoleEnableFunctionName();
        where = &r;
        if (g_calls != 1 || g_len != -1 || g_text == 0 || strcmp(g_text, "EnableScreenEffectDisplacementRenderer") != 0 || g_slot != where) {
            printf("FAIL_00b88f80 calls=%d len=%ld text=%s\n", g_calls, g_len, g_text ? g_text : "(null)");
            return 1;
        }
    }
    printf("CONSOLE_ENABLE_NAME_OK\n");
    return 0;
}
