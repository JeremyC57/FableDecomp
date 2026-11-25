// Behaviour test for 0x007711C0 (NF388 literal audit): links the row object and asserts the string the row hands to
// CCharString::CCharString(char const*, int): "CTCThingUIDDummyForSerialisation", length -1, one call, into the hidden return slot.
#include <stdio.h>
#include <string.h>
// CTCThingUIDDummyForSerialisation::GetClassTypeName @ 007711c0
// __fastcall, returns CCharString by value (hidden retptr on stack, ret 4)

class CCharString { public:
    char* p;
    CCharString(const char* s, long n);
};

class CTCThingUIDDummyForSerialisation { public:
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
        CCharString r = ((CTCThingUIDDummyForSerialisation*)g_storage)->CTCThingUIDDummyForSerialisation::GetClassTypeName();
        where = &r;
        if (g_calls != 1 || g_len != -1 || g_text == 0 || strcmp(g_text, "CTCThingUIDDummyForSerialisation") != 0 || g_slot != where) {
            printf("FAIL_007711c0 calls=%d len=%ld text=%s\n", g_calls, g_len, g_text ? g_text : "(null)");
            return 1;
        }
    }
    printf("007711c0_TEST PASS\n");
    return 0;
}
