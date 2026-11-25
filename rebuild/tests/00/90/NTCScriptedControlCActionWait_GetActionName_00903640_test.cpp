// Behaviour test for 0x00903640 (NF388 literal audit): links the row object and asserts the string the row hands to
// CCharString::CCharString(char const*, int): "CActionWait", length -1, one call, into the hidden return slot.
#include <stdio.h>
#include <string.h>
// NTCScriptedControl::CActionWait::GetActionName() -- the scripted action's name for logging: "CActionWait"
// Leaf: ?GetActionName@CActionWait@NTCScriptedControl@@
// Renamed (fistp pass, 2026-10-24) from NTCScriptedControl::CActionDoCreatureAction::GetActionName @ 00/90/CActionDoCreatureAction_GetActionName_00903640.cpp: vtable slot 4 of NTCScriptedControl::CActionWait (the slot CActionDoCreatureAction::GetActionName occupies, Ego_r.pdb); the returned literal is the class name "CActionWait"
// (was: NTCScriptedControl::CActionWait::GetActionName  @ 0x00903640)
// Returns a CCharString by value: constructs it from a fixed literal + (-1).
// __fastcall struct-return: the hidden return buffer is a stack arg cleaned by ret 4.

class CCharString { public:
    char* m_data;
    CCharString(const char* s, long n);
};

namespace NTCScriptedControl {
struct CActionWait {
    CCharString GetActionName();
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
        CCharString r = ((NTCScriptedControl::CActionWait*)g_storage)->NTCScriptedControl::CActionWait::GetActionName();
        where = &r;
        if (g_calls != 1 || g_len != -1 || g_text == 0 || strcmp(g_text, "CActionWait") != 0 || g_slot != where) {
            printf("FAIL_00903640 calls=%d len=%ld text=%s\n", g_calls, g_len, g_text ? g_text : "(null)");
            return 1;
        }
    }
    printf("00903640_TEST PASS\n");
    return 0;
}
