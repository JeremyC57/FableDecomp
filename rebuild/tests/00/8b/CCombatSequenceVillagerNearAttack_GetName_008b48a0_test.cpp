// Behaviour test for 0x008B48A0 (NF388 literal audit): links the row object and asserts the string the row hands to
// CCharString::CCharString(char const*, int): "COMBAT_SEQUENCE_VILLAGER_NEAR_ATTACK", length -1, one call, into the hidden return slot.
#include <stdio.h>
#include <string.h>
// CCombatSequence_VillagerNearAttack::GetName
// Returns a CCharString by value built from a static string literal.
// Retail: push -1; push 0x1278cb8; ecx=retbuf; call CCharString::ctor; return retbuf.

class CCharString {
public:
    CCharString(const char* s, long len);
    char* m_data;
};

struct CActionDoCreatureAction;

class CCombatSequence_VillagerNearAttack {
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
        CCharString r = ((CCombatSequence_VillagerNearAttack*)g_storage)->CCombatSequence_VillagerNearAttack::GetName();
        where = &r;
        if (g_calls != 1 || g_len != -1 || g_text == 0 || strcmp(g_text, "COMBAT_SEQUENCE_VILLAGER_NEAR_ATTACK") != 0 || g_slot != where) {
            printf("FAIL_008b48a0 calls=%d len=%ld text=%s\n", g_calls, g_len, g_text ? g_text : "(null)");
            return 1;
        }
    }
    printf("GETNAME_OK\n");
    return 0;
}
