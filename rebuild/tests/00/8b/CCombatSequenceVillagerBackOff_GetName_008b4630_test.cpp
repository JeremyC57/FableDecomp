// Behaviour test for 0x008B4630 (NF388 literal audit): links the row object and asserts the string the row hands to
// CCharString::CCharString(char const*, int): "COMBAT_SEQUENCE_VILLAGER_BACK_OFF", length -1, one call, into the hidden return slot.
#include <stdio.h>
#include <string.h>
// Leaf: ?GetName@CCombatSequence_VillagerBackOff@@UBE?AVCCharString@@XZ
// Class-model move (NF325, 2026-11-24): defined on the real class of engine/CCombatSequence_VillagerBackOff.h under FABLE_RETAIL_CCOMBATSEQUENCE_VILLAGERBACKOFF_VIRTUAL_MODEL (the opt-in polymorphic model;
//   was CCombatSequence_VillagerBackOff_Methods::GetName); the retail RTTI vftable placement names this PDB member here.
#define FABLE_RETAIL_CCOMBATSEQUENCE_VILLAGERBACKOFF_VIRTUAL_MODEL
#define FABLE_RETAIL_CCOMBATSEQUENCEBASE
#include "engine/CCombatSequence_VillagerBackOff.h"

class CCharString { public: char* m_pData; CCharString(const char*, long); };


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
        CCharString r = ((CCombatSequence_VillagerBackOff*)g_storage)->CCombatSequence_VillagerBackOff::GetName();
        where = &r;
        if (g_calls != 1 || g_len != -1 || g_text == 0 || strcmp(g_text, "COMBAT_SEQUENCE_VILLAGER_BACK_OFF") != 0 || g_slot != where) {
            printf("FAIL_008b4630 calls=%d len=%ld text=%s\n", g_calls, g_len, g_text ? g_text : "(null)");
            return 1;
        }
    }
    printf("GETNAME_OK\n");
    return 0;
}
