// Behaviour test for 0x008B4350 (NF388 literal audit): links the row object and asserts the string the row hands to
// CCharString::CCharString(char const*, int): "COMBAT_SEQUENCE_TROLL_ROCK_PURSUE_NO_ZONE", length -1, one call, into the hidden return slot.
#include <stdio.h>
#include <string.h>
// COMBAT_SEQUENCE_TROLL_ROCK_PURSUE_NO_ZONE::GetName  @ 0x008b4350
// virtual CCharString GetName() const  -- returns a fixed name string by value.

class CCharString {
public:
    // __fastcall ctor from a C string + explicit length (-1 = strlen).
    CCharString(const char* s, long len);
private:
    char* m_pData;
};

class CCombatSequence_TrollRockPursueNoZone {
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
        CCharString r = ((CCombatSequence_TrollRockPursueNoZone*)g_storage)->CCombatSequence_TrollRockPursueNoZone::GetName();
        where = &r;
        if (g_calls != 1 || g_len != -1 || g_text == 0 || strcmp(g_text, "COMBAT_SEQUENCE_TROLL_ROCK_PURSUE_NO_ZONE") != 0 || g_slot != where) {
            printf("FAIL_008b4350 calls=%d len=%ld text=%s\n", g_calls, g_len, g_text ? g_text : "(null)");
            return 1;
        }
    }
    printf("GETNAME_OK\n");
    return 0;
}
