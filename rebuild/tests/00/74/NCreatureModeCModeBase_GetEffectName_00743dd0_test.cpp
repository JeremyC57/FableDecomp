// Behaviour test for 0x00743DD0 (NF388 literal audit): links the row object and asserts the string the row hands to
// CCharString::CCharString(char const*, int): "", length -1, one call, into the hidden return slot.
#include <stdio.h>
#include <string.h>
// NCreatureMode::CModeBase::GetEffectName() -- the default effect name of a creature mode: the empty string
// Leaf: ?GetEffectName@CModeBase@NCreatureMode@@
// Renamed (fistp pass, 2026-10-24) from NTCScriptedControl::CActionDoCreatureAction::GetActionName @ 00/74/CActionDoCreatureAction_GetActionName_00743dd0.cpp: shared by several NCreatureMode classes at vtable slot 14 (CModeWalkInAllDirections, CModeHoverBase, ...) = the base default of the slot CModeHoverNormal::GetEffectName (0x00744110) overrides; returns ""; the declaring base is named from the shared vtable position, not from a PDB slot dump
// (was: NCreatureMode::CModeBase::GetEffectName @ 0x00743dd0)
// Real __fastcall member returning CCharString by value: this in ECX,
// hidden struct-return pointer on the stack (ret 4). this is unused.

class CCharString {
public:
    CCharString(const char* text, long length);
    long storage_;
};

namespace NCreatureMode {
class CModeBase {
public:
    CCharString GetEffectName();
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
        CCharString r = ((NCreatureMode::CModeBase*)g_storage)->NCreatureMode::CModeBase::GetEffectName();
        where = &r;
        if (g_calls != 1 || g_len != -1 || g_text == 0 || strcmp(g_text, "") != 0 || g_slot != where) {
            printf("FAIL_00743dd0 calls=%d len=%ld text=%s\n", g_calls, g_len, g_text ? g_text : "(null)");
            return 1;
        }
    }
    printf("00743dd0_TEST PASS\n");
    return 0;
}
