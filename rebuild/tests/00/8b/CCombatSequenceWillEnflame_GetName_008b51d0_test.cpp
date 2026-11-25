// Behaviour test for 0x008B51D0 (NF388 literal audit): links the row object and asserts the string the row hands to
// CCharString::CCharString(char const*, int): "COMBAT_SEQUENCE_WILL_ENFLAME", length -1, one call, into the hidden return slot.
#include <stdio.h>
#include <string.h>
// CCombatSequence_WillEnflame::GetName  (retail 0x008b51d0)
// virtual CCharString GetName() const;  -- returns a CCharString by value built
// from a fixed string literal via CCharString::CCharString(char const*, int).

class CCharString {
public:
    // ctor at 0x0099ebf0 : CCharString(char const* s, int n)
    CCharString(const char* s, long n);
private:
    char* m_p;
};

class CCombatSequence_WillEnflame {
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
        CCharString r = ((CCombatSequence_WillEnflame*)g_storage)->CCombatSequence_WillEnflame::GetName();
        where = &r;
        if (g_calls != 1 || g_len != -1 || g_text == 0 || strcmp(g_text, "COMBAT_SEQUENCE_WILL_ENFLAME") != 0 || g_slot != where) {
            printf("FAIL_008b51d0 calls=%d len=%ld text=%s\n", g_calls, g_len, g_text ? g_text : "(null)");
            return 1;
        }
    }
    printf("PASS_WILLENFLAME_GETNAME\n");
    return 0;
}
