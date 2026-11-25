// Behaviour test for 0x008B5350 (NF388 literal audit): links the row object and asserts the string the row hands to
// CCharString::CCharString(char const*, int): "COMBAT_SEQUENCE_WILL_FORCE_PUSH_LONG_RANGE", length -1, one call, into the hidden return slot.
#include <stdio.h>
#include <string.h>
// CCombatSequence_WillForcePushLongRange::GetName  @ 008b5350
// __fastcall, returns CCharString by value (hidden return-buffer ptr = first stack arg).
// Body: constructs a CCharString from a string literal with length -1, returns the buffer.

class CCharString {
public:
    // ctor from (const char*, long) — the retail call at 0x99ebf0.
    CCharString(const char* s, long n);
private:
    char* m_p;
};

class CActionDoCreatureAction;

class CCombatSequence_WillForcePushLongRange {
public:
    virtual CCharString GetName() const;
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
        CCharString r = ((CCombatSequence_WillForcePushLongRange*)g_storage)->CCombatSequence_WillForcePushLongRange::GetName();
        where = &r;
        if (g_calls != 1 || g_len != -1 || g_text == 0 || strcmp(g_text, "COMBAT_SEQUENCE_WILL_FORCE_PUSH_LONG_RANGE") != 0 || g_slot != where) {
            printf("FAIL_008b5350 calls=%d len=%ld text=%s\n", g_calls, g_len, g_text ? g_text : "(null)");
            return 1;
        }
    }
    printf("GETNAME_OK\n");
    return 0;
}
