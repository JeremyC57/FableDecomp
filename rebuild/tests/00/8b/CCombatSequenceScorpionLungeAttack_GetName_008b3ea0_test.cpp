// Behaviour test for 0x008B3EA0 CCombatSequence_ScorpionLungeAttack::GetName: "COMBAT_SEQUENCE_SCORPION_LUNGE_ATTACK" (retail 0x012789A0), length -1.
// NF418 (2026-11-25): linked behaviour test: the row function is declared here, not defined, so the test links the row object and the
// callees below are defined under the row's spellings; it asserts the literal text the row passes (a mutated literal fails).
#include <cstdio>
#include <cstring>
#define FABLE_RETAIL_CCOMBATSEQUENCE_SCORPIONLUNGEATTACK_VIRTUAL_MODEL
#define FABLE_RETAIL_CCOMBATSEQUENCEBASE
#include "engine/CCombatSequence_ScorpionLungeAttack.h"
class CCharString { public: char* m_data; CCharString(const char*, long); };
static const char* g_s = 0; static long g_n = 0; static int g_calls = 0; static const void* g_this = 0;
static char g_storage[1024];
CCharString::CCharString(const char* s, long n) { g_s = s; g_n = n; ++g_calls; g_this = this; m_data = 0; }
int main()
{
    const void* where = 0;
    {
        CCharString r = ((CCombatSequence_ScorpionLungeAttack*)g_storage)->CCombatSequence_ScorpionLungeAttack::GetName();
        where = &r;
        if (g_calls != 1 || g_n != -1 || g_this != where || g_s == 0 || std::strcmp(g_s, "COMBAT_SEQUENCE_SCORPION_LUNGE_ATTACK") != 0) {
            std::printf("GETNAME_FAIL calls=%d len=%ld text=%s\n", g_calls, g_n, g_s ? g_s : "(null)"); return 1;
        }
    }
    std::printf("GETNAME_OK\n");
    return 0;
}
