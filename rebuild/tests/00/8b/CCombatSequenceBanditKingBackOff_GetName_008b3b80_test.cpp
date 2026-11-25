// Behaviour test for 0x008B3B80 CCombatSequence_BanditKingBackOff::GetName: "COMBAT_SEQUENCE_BANDIT_KING_BACK_OFF" (retail 0x012788B0), length -1.
// NF418 (2026-11-25): linked behaviour test: the row function is declared here, not defined, so the test links the row object and the
// callees below are defined under the row's spellings; it asserts the literal text the row passes (a mutated literal fails).
#include <cstdio>
#include <cstring>
class CCharString { public: int d; CCharString(const char* s, long n); };
struct CCombatSequence_BanditKingBackOff { int x; CCharString GetName(); };
static const char* g_s = 0; static long g_n = 0; static int g_calls = 0; static const void* g_this = 0;
CCharString::CCharString(const char* s, long n) { d = 0x55; g_s = s; g_n = n; ++g_calls; g_this = this; }
int main()
{
    CCombatSequence_BanditKingBackOff obj; obj.x = 3;
    CCharString r = obj.GetName();
    if (r.d == 0x55 && g_calls == 1 && g_n == -1 && g_this == &r && g_s != 0 && std::strcmp(g_s, "COMBAT_SEQUENCE_BANDIT_KING_BACK_OFF") == 0) {
        std::printf("008b3b80_TEST PASS\n"); return 0;
    }
    std::printf("FAIL n=%ld s=%s\n", g_n, g_s ? g_s : "(null)");
    return 1;
}
