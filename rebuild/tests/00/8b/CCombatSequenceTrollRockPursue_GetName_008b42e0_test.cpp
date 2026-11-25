// Behaviour test for 0x008B42E0 (NF388 literal audit): links the row object and asserts the string the row hands to
// CCharString::CCharString(char const*, int): "COMBAT_SEQUENCE_TROLL_ROCK_PURSUE", length -1, one call, into the hidden return slot.
#include <stdio.h>
#include <string.h>
// Leaf: ?GetName@CCombatSequence_TrollRockPursue@@UBE?AVCCharString@@XZ
// Class-model move (NF325, 2026-11-24): defined on the real class of engine/CCombatSequence_TrollRockPursue.h under FABLE_RETAIL_CCOMBATSEQUENCE_TROLLROCKPURSUE_VIRTUAL_MODEL (the opt-in polymorphic model;
//   was CCombatSequence_TrollRockPursue_Methods::GetName(CCharString* __ret) calling the stand-in CCharString::Init); the retail RTTI vftable placement names this PDB member here.
//   The by-value PDB return builds the result through CCharString::CCharString(const char*, long) (0x0099EBF0, both PDBs ??0CCharString@@QAE@PBDJ@Z, its landed self row).
// Retail: builds a CCharString from a string literal (len=-1) into the hidden
// return buffer (esi) and returns that buffer.
#define FABLE_RETAIL_CCOMBATSEQUENCE_TROLLROCKPURSUE_VIRTUAL_MODEL
#define FABLE_RETAIL_CCOMBATSEQUENCEBASE
#include "engine/CCombatSequence_TrollRockPursue.h"  // retyped onto the PDB layout; byte parity re-verified
class CCharString {
public:
    CCharString(const char* s, long len);
    void* m_data;
};

// String literal lives at 0x1278aec in retail.
static const char* const s_name = "COMBAT_SEQUENCE_TROLL_ROCK_PURSUE";



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
        CCharString r = ((CCombatSequence_TrollRockPursue*)g_storage)->CCombatSequence_TrollRockPursue::GetName();
        where = &r;
        if (g_calls != 1 || g_len != -1 || g_text == 0 || strcmp(g_text, "COMBAT_SEQUENCE_TROLL_ROCK_PURSUE") != 0 || g_slot != where) {
            printf("FAIL_008b42e0 calls=%d len=%ld text=%s\n", g_calls, g_len, g_text ? g_text : "(null)");
            return 1;
        }
    }
    printf("GETNAME_TROLLROCK_OK\n");
    return 0;
}
