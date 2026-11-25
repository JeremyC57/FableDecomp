// Behaviour test for 0x008B3910 (NF388 literal audit): links the row object and asserts the string the row hands to
// CCharString::CCharString(char const*, int): "COMBAT_SEQUENCE_BANDIT_IDLE_SHORT", length -1, one call, into the hidden return slot.
#include <stdio.h>
#include <string.h>
// CCombatSequence_BanditIdleNear::GetName  @ 008b3910
// __fastcall, returns CCharString by value; param CActionDoCreatureAction* (unused).
// Body: builds CCharString("COMBAT_SEQUENCE_BANDIT_IDLE_SHORT", -1) into the hidden return buffer, returns it.

class CCharString {
public:
    char *m_data;
    CCharString(const char *s, long n);   // ctor @ 0x99ebf0
};

class CCombatSequence_BanditIdleNear {
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
        CCharString r = ((CCombatSequence_BanditIdleNear*)g_storage)->CCombatSequence_BanditIdleNear::GetName();
        where = &r;
        if (g_calls != 1 || g_len != -1 || g_text == 0 || strcmp(g_text, "COMBAT_SEQUENCE_BANDIT_IDLE_SHORT") != 0 || g_slot != where) {
            printf("FAIL_008b3910 calls=%d len=%ld text=%s\n", g_calls, g_len, g_text ? g_text : "(null)");
            return 1;
        }
    }
    printf("PASS_BANDITIDLE\n");
    return 0;
}
