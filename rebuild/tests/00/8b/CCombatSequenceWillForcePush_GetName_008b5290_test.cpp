// Behaviour test for 0x008B5290 (NF388 literal audit): links the row object and asserts the string the row hands to
// CCharString::CCharString(char const*, int): "COMBAT_SEQUENCE_WILL_FORCE_PUSH", length -1, one call, into the hidden return slot.
#include <stdio.h>
#include <string.h>
// CCombatSequence_WillForcePush::GetName  @ 0x008b5290
// virtual CCharString GetName(void) const  -- returns a CCharString by value
// built from a string literal via the CCharString(char const*, int) ctor @ 0x0099ebf0.

class CCharString {
public:
    CCharString(const char* s, long len);   // 0x0099ebf0
    CCharString(const CCharString& o);
private:
    char* m_p;
    int   m_len;
    int   m_cap;
};

class CActionDoCreatureAction;

class CCombatSequence_WillForcePush {
public:
    virtual CCharString GetName(void) const;
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
        CCharString r = ((CCombatSequence_WillForcePush*)g_storage)->CCombatSequence_WillForcePush::GetName();
        where = &r;
        if (g_calls != 1 || g_len != -1 || g_text == 0 || strcmp(g_text, "COMBAT_SEQUENCE_WILL_FORCE_PUSH") != 0 || g_slot != where) {
            printf("FAIL_008b5290 calls=%d len=%ld text=%s\n", g_calls, g_len, g_text ? g_text : "(null)");
            return 1;
        }
    }
    printf("GETNAME_PASS_WILLFORCEPUSH\n");
    return 0;
}
