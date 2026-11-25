// Behaviour test for 0x00903680 (NF388 literal audit): links the row object and asserts the string the row hands to
// CCharString::CCharString(char const*, int): "CActionStopScripting", length -1, one call, into the hidden return slot.
#include <stdio.h>
#include <string.h>
// NTCScriptedControl::CActionStopScripting::GetActionName() -- the scripted action's name for logging: "CActionStopScripting"
// Leaf: ?GetActionName@CActionStopScripting@NTCScriptedControl@@
// Renamed (fistp pass, 2026-10-24) from NTCScriptedControl::CActionDoCreatureAction::GetActionName @ 00/90/CActionDoCreatureAction_GetActionName_00903680.cpp: vtable slot 4 of NTCScriptedControl::CActionStopScripting (the slot CActionDoCreatureAction::GetActionName occupies, Ego_r.pdb); the returned literal is the class name "CActionStopScripting"
// (was: Faithful VC7.1 reconstruction of)
//   NTCScriptedControl::CActionStopScripting::GetActionName  (0x00903680)
//
// Retail disasm:
//   push esi
//   mov  esi, [esp+8]        ; esi = hidden return buffer (CCharString*)
//   push -1
//   push 0x128d7a0           ; const char* literal
//   mov  ecx, esi            ; this = return buffer
//   call CCharString::CCharString(const char*, long)  ; 0x99ebf0
//   mov  eax, esi
//   pop  esi
//   ret  4
//
// A GetActionName() const returning a CCharString by value, NRVO'd into the
// caller-supplied return buffer, initialised from a string literal with an
// explicit length arg of -1.

class CCharString
{
public:
    // Declaration only: defined in another TU (retail 0x99ebf0), so the
    // compiler MUST emit a real call, matching retail's push/push/call.
    CCharString(const char* s, long len);
private:
    char* m_data;
};

namespace NTCScriptedControl {
class CActionStopScripting
{
public:
    CCharString GetActionName() const;
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
        CCharString r = ((NTCScriptedControl::CActionStopScripting*)g_storage)->NTCScriptedControl::CActionStopScripting::GetActionName();
        where = &r;
        if (g_calls != 1 || g_len != -1 || g_text == 0 || strcmp(g_text, "CActionStopScripting") != 0 || g_slot != where) {
            printf("FAIL_00903680 calls=%d len=%ld text=%s\n", g_calls, g_len, g_text ? g_text : "(null)");
            return 1;
        }
    }
    printf("00903680_TEST PASS\n");
    return 0;
}
