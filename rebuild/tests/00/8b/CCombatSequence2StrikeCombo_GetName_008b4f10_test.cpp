// Behaviour test for 0x008B4F10 CCombatSequence2StrikeCombo::GetName (NF388 literal audit): links the row object and asserts the
// string it hands to CCharString::CCharString(char const*, int) (modelled as the member `ctor`): "COMBAT_SEQUENCE_TWO_STRIKE",
// length -1, into the caller's return slot (retail pushes 0x01278E74; the HEAD row passed "2StrikeCombo").
#include <cstdio>
#include <cstring>
class CCharString { public:
    char* p;
    void ctor(const char* s, int n);
};
static int g_calls = 0; static int g_len = 0;
void CCharString::ctor(const char* s, int n) { p = (char*)s; g_len = n; ++g_calls; }
struct CCombatSequence2StrikeCombo { void* vtbl; };
extern "C" CCharString* __fastcall CCombatSequence2StrikeCombo_GetName(CCombatSequence2StrikeCombo* sequence, int edx, CCharString* ret);
int main()
{
    CCombatSequence2StrikeCombo obj; obj.vtbl = 0;
    CCharString out; out.p = 0;
    CCharString* r = CCombatSequence2StrikeCombo_GetName(&obj, 0, &out);
    if (r != &out || g_calls != 1 || g_len != -1 || out.p == 0 || std::strcmp(out.p, "COMBAT_SEQUENCE_TWO_STRIKE") != 0) {
        std::printf("FAIL_008b4f10 %s\n", out.p ? out.p : "(null)"); return 1;
    }
    std::printf("STRIKECOMBO_GETNAME_OK\n");
    return 0;
}
