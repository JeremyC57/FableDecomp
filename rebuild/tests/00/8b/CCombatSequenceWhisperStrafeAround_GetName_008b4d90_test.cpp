// Behaviour test for 0x008B4D90 (NF388 literal audit): links the row object and asserts the string the row hands to
// CCharString::CCharString(char const*, int): "COMBAT_SEQUENCE_WHISPER_STRAFE_AROUND", length -1, one call, into the hidden return slot.
#include <stdio.h>
#include <string.h>
// CCombatSequenceWhisperStrafeAround_Methods::GetName  (retail 0x008b4d90)
// Virtual method returning CCharString by value from a fixed string literal.

// Leaf: ?GetName@CCombatSequenceWhisperStrafeAround@@UBE?AVCCharString@@XZ
// Class-model move (NF325, 2026-11-24): defined on the real class of engine/CCombatSequenceWhisperStrafeAround.h under FABLE_RETAIL_CCOMBATSEQUENCEWHISPERSTRAFEAROUND_VIRTUAL_MODEL (the opt-in polymorphic model;
//   was CCombatSequenceWhisperStrafeAround_Methods::GetName); the retail RTTI vftable placement names this PDB member here.
#define FABLE_RETAIL_CCOMBATSEQUENCEWHISPERSTRAFEAROUND_VIRTUAL_MODEL
#define FABLE_RETAIL_CCOMBATSEQUENCEBASE
#include "engine/CCombatSequenceWhisperStrafeAround.h"  // retyped onto the PDB layout; byte parity re-verified
class CCharString { public:
    char* m_buf;
    // CCharString::CCharString(char const*, int)  @ 0x0099ebf0  (__fastcall)
    CCharString(const char* s, long n);
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
        CCharString r = ((CCombatSequenceWhisperStrafeAround*)g_storage)->CCombatSequenceWhisperStrafeAround::GetName();
        where = &r;
        if (g_calls != 1 || g_len != -1 || g_text == 0 || strcmp(g_text, "COMBAT_SEQUENCE_WHISPER_STRAFE_AROUND") != 0 || g_slot != where) {
            printf("FAIL_008b4d90 calls=%d len=%ld text=%s\n", g_calls, g_len, g_text ? g_text : "(null)");
            return 1;
        }
    }
    printf("GETNAME_OK\n");
    return 0;
}
