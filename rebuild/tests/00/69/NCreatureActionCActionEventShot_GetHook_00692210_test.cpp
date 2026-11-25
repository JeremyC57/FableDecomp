// Behaviour test for 0x00692210 (NF388 literal audit): links the row object and asserts the string the row hands to
// CCharString::CCharString(char const*, int): "SHOT", length -1, one call, into the hidden return slot.
#include <stdio.h>
#include <string.h>
// NCreatureAction::CActionEventShot::GetHook() -- the animation hook name this creature-action event listens for: "SHOT"
// Leaf: ?GetHook@CActionEventShot@NCreatureAction@@
// Renamed (fistp pass, 2026-10-24) from NTCScriptedControl::CActionDoCreatureAction::GetActionName @ 00/69/CActionDoCreatureAction_GetActionName_00692210.cpp: vtable slot 2 of NCreatureAction::CActionEventShot; Ego_r.pdb lists GetHook as a per-class method of the CActionEvent classes (Alloc/Dispatch/GetHook/SetDataFromString) and the literal "SHOT" is an animation hook tag; method name inferred from that list (not from a slot dump)
// (was: NCreatureAction::CActionEventShot::GetHook - retail 0x00692210)
// Returns a fixed CCharString by value (NRV hidden return pointer).

class CCharString {
public:
    CCharString(const char* s, long n);   // extern ctor -> real call (matches call 0x99ebf0)
    char* m_data;
};

namespace NCreatureAction {
class CActionEventShot {
public:
    CCharString GetHook();
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
        CCharString r = ((NCreatureAction::CActionEventShot*)g_storage)->NCreatureAction::CActionEventShot::GetHook();
        where = &r;
        if (g_calls != 1 || g_len != -1 || g_text == 0 || strcmp(g_text, "SHOT") != 0 || g_slot != where) {
            printf("FAIL_00692210 calls=%d len=%ld text=%s\n", g_calls, g_len, g_text ? g_text : "(null)");
            return 1;
        }
    }
    printf("00692210_TEST PASS\n");
    return 0;
}
