// Behaviour test for 0x00B4B6F0 (NF388 literal audit): links the row object and asserts the string the row hands to
// CCharString::CCharString(char const*, int): "Enable2DPrimitives", length -1, one call, into the hidden return slot.
#include <stdio.h>
#include <string.h>
// CEnginePrimitiveRenderer2D::GetConsoleEnableFunctionName
// Retail 0x00b4b6f0 (__fastcall, returns CCharString by value).
//
// Body just constructs a constant CCharString("...", -1) into the hidden
// return buffer and returns it. The `this` pointer is unused.

// Minimal CCharString: a single data pointer, ctor(const char*, long).
// The ctor is an external engine routine (retail 0x0099ebf0). We only need
// its declaration so VC7.1 emits the push -1 / push <lit> / mov ecx / call
// sequence; the call target + literal address are reloc slots the harness masks.
class CCharString
{
public:
    CCharString(const char* s, long n);
    char* m_data;
};

class CEnginePrimitiveRenderer2D
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
int main(void)
{
    const void* where = 0;
    {
        CCharString r = ((CEnginePrimitiveRenderer2D*)g_storage)->CEnginePrimitiveRenderer2D::GetConsoleEnableFunctionName();
        where = &r;
        if (g_calls != 1 || g_len != -1 || g_text == 0 || strcmp(g_text, "Enable2DPrimitives") != 0 || g_slot != where) {
            printf("FAIL_00b4b6f0 calls=%d len=%ld text=%s\n", g_calls, g_len, g_text ? g_text : "(null)");
            return 1;
        }
    }
    printf("GETCONSOLEENABLE_OK\n");
    return 0;
}
