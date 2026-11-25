// Behaviour test for 0x00BDBF00 (NF388 literal audit): links the row object and asserts the string the row hands to
// CCharString::CCharString(char const*, int): "LayoutBasic", length -1, one call, into the hidden return slot.
#include <stdio.h>
#include <string.h>
// LayoutBasic::PeekName  @ 0x00bdbf00
// __fastcall, returns CCharString by value (hidden ret buffer as stack arg).
// Body: construct a CCharString from a fixed string literal with length -1.

class CCharString {
public:
    CCharString(const char* text, long length);
    // one pointer member so sizeof matches the engine's small string handle
    void* storage_;
};

class CEngineVSConstantLayoutBasic {
public:
    CCharString PeekName();
};

// The fixed literal the engine passes (address 0x012a7d48 in retail).
static const char* const kName = "LayoutBasic";


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
        CCharString r = ((CEngineVSConstantLayoutBasic*)g_storage)->CEngineVSConstantLayoutBasic::PeekName();
        where = &r;
        if (g_calls != 1 || g_len != -1 || g_text == 0 || strcmp(g_text, "LayoutBasic") != 0 || g_slot != where) {
            printf("FAIL_00bdbf00 calls=%d len=%ld text=%s\n", g_calls, g_len, g_text ? g_text : "(null)");
            return 1;
        }
    }
    printf("PEEKNAME_OK\n");
    return 0;
}
