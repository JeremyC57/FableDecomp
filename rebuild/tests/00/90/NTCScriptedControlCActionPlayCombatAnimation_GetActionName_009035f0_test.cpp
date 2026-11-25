// Behaviour test for 0x009035F0 (NF388 literal audit): links the row object and asserts the string the row hands to
// CCharString::CCharString(char const*, int): "CActionPlayCombatAnimation", length -1, one call, into the hidden return slot.
#include <stdio.h>
#include <string.h>
// NTCScriptedControl::CActionPlayCombatAnimation::GetActionName() -- the scripted action's name for logging: "CActionPlayCombatAnimation"
// Leaf: ?GetActionName@CActionPlayCombatAnimation@NTCScriptedControl@@
// Renamed (fistp pass, 2026-10-24) from NTCScriptedControl::CActionDoCreatureAction::GetActionName @ 00/90/CActionDoCreatureAction_GetActionName_009035f0.cpp: vtable slot 4 of NTCScriptedControl::CActionPlayCombatAnimation (the slot CActionDoCreatureAction::GetActionName occupies, Ego_r.pdb); the returned literal is the class name "CActionPlayCombatAnimation"
// (was: NTCScriptedControl::CActionPlayCombatAnimation::GetActionName  (retail 0x009035f0))
// __fastcall, returns CCharString by value (hidden retbuf as first stack arg).
// Body: return CCharString("...", -1);  -- 'this' unused.

class CCharString {
public:
    CCharString(const char *s, long n);   // 0x0099ebf0, out-of-line ctor
private:
    char *m_p;
};

namespace NTCScriptedControl {
class CActionPlayCombatAnimation {
public:
    CCharString GetActionName();
    // some data so 'this' is a real object (unused by the body)
    int m_dummy;
};
}



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
        CCharString r = ((NTCScriptedControl::CActionPlayCombatAnimation*)g_storage)->NTCScriptedControl::CActionPlayCombatAnimation::GetActionName();
        where = &r;
        if (g_calls != 1 || g_len != -1 || g_text == 0 || strcmp(g_text, "CActionPlayCombatAnimation") != 0 || g_slot != where) {
            printf("FAIL_009035f0 calls=%d len=%ld text=%s\n", g_calls, g_len, g_text ? g_text : "(null)");
            return 1;
        }
    }
    printf("009035f0_TEST PASS\n");
    return 0;
}
