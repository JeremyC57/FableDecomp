// Behaviour test for 0x00905160 (NF388 literal audit): links the row object and asserts the string the row hands to
// CCharString::CCharString(char const*, int): "CActionWaitForThingToFinishPerformingTasks", length -1, one call, into the hidden return slot.
#include <stdio.h>
#include <string.h>
// NTCScriptedControl::CActionWaitForThingToFinishPerformingTasks::GetActionName() -- the scripted action's name for logging: "CActionWaitForThingToFinishPerformingTasks"
// Leaf: ?GetActionName@CActionWaitForThingToFinishPerformingTasks@NTCScriptedControl@@
// Renamed (fistp pass, 2026-10-24) from NTCScriptedControl::CActionDoCreatureAction::GetActionName @ 00/90/CActionDoCreatureAction_GetActionName_00905160.cpp: vtable slot 4 of NTCScriptedControl::CActionWaitForThingToFinishPerformingTasks (the slot CActionDoCreatureAction::GetActionName occupies, Ego_r.pdb); the returned literal is the class name "CActionWaitForThingToFinishPerformingTasks"
// (was: NTCScriptedControl::CActionWaitForThingToFinishPerformingTasks::GetActionName  @ 0x00905160)
// Struct-return __fastcall member: returns CCharString("CActionWaitForThingToFinishPerformingTasks", -1) by value.
// The 'this' pointer is unused by the body.

class CCharString { public:
    char* m_pData;
    CCharString(const char* s, long n);
};

namespace NTCScriptedControl {
struct CActionWaitForThingToFinishPerformingTasks {
    int dummy;
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
        CCharString r = ((NTCScriptedControl::CActionWaitForThingToFinishPerformingTasks*)g_storage)->NTCScriptedControl::CActionWaitForThingToFinishPerformingTasks::GetActionName();
        where = &r;
        if (g_calls != 1 || g_len != -1 || g_text == 0 || strcmp(g_text, "CActionWaitForThingToFinishPerformingTasks") != 0 || g_slot != where) {
            printf("FAIL_00905160 calls=%d len=%ld text=%s\n", g_calls, g_len, g_text ? g_text : "(null)");
            return 1;
        }
    }
    printf("00905160_TEST PASS\n");
    return 0;
}
