// Behaviour test for 0x008B3B00 (NF388 literal audit): links the row object and asserts the string the row hands to
// CCharString::CCharString(char const*, int): "COMBAT_SEQUENCE_BANDIT_KING_ADVANCE", length -1, one call, into the hidden return slot.
#include <stdio.h>
#include <string.h>
// CCombatSequence_BanditKingAdvance_Methods::GetName -- returns CCharString by value.
// Retail: builds a CCharString from a string literal (len = -1) into the
// hidden return buffer and returns it. 'this' is unused.

// Leaf: ?GetName@CCombatSequence_BanditKingAdvance@@UBE?AVCCharString@@XZ
// Class-model move (NF325, 2026-11-24): defined on the real class of engine/CCombatSequence_BanditKingAdvance.h under FABLE_RETAIL_CCOMBATSEQUENCE_BANDITKINGADVANCE_VIRTUAL_MODEL (the opt-in polymorphic model;
//   was CCombatSequence_BanditKingAdvance_Methods::GetName); the retail RTTI vftable placement names this PDB member here.
#define FABLE_RETAIL_CCOMBATSEQUENCE_BANDITKINGADVANCE_VIRTUAL_MODEL
#define FABLE_RETAIL_CCOMBATSEQUENCEBASE
#include "engine/CCombatSequence_BanditKingAdvance.h"  // retyped onto the PDB layout; byte parity re-verified
class CCharString { public:
    char* m_ptr;
    CCharString(const char* s, long len);
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
        CCharString r = ((CCombatSequence_BanditKingAdvance*)g_storage)->CCombatSequence_BanditKingAdvance::GetName();
        where = &r;
        if (g_calls != 1 || g_len != -1 || g_text == 0 || strcmp(g_text, "COMBAT_SEQUENCE_BANDIT_KING_ADVANCE") != 0 || g_slot != where) {
            printf("FAIL_008b3b00 calls=%d len=%ld text=%s\n", g_calls, g_len, g_text ? g_text : "(null)");
            return 1;
        }
    }
    printf("GETNAME_OK\n");
    return 0;
}
