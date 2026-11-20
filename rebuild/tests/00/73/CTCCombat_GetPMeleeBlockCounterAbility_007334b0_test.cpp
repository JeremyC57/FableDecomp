// Behaviour test for 0x007334B0 (links the REAL row object; NF138): CTCCombat::GetPMeleeBlockCounterAbility() const copies
// PBlockCounterAbility (+0x48) into the result and adds a reference when it is non-null; the copy releases it again.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCOMBATABILITY
#define FABLE_RETAIL_CCOMBATABILITYDATA
#define FABLE_RETAIL_CTCCOMBAT
#define FABLE_RETAIL_CTCCOMBAT_GETPMELEEBLOCKCOUNTERABILITY
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCCombat.h"
#include <stdio.h>
int main()
{
    unsigned long def[8] = { 0, 5, 0, 0, 0, 0, 0, 0 };            // the pointee: vtable word, IVRefCount +4 (no Delete reached)
    unsigned long combat[0x98 / 4];
    for (int i = 0; i < 0x98 / 4; ++i) combat[i] = 0;
    combat[0x48 / 4] = (unsigned long)def;
    const CTCCombat* c = (const CTCCombat*)combat;
    int ok = 1;
    {
        CDefPointer<const CMeleeCombatAbilityDef> p = c->GetPMeleeBlockCounterAbility();
        if ((void*)p.InternalGetObject() != (void*)def || def[1] != 6) ok = 0;
    }
    if (def[1] != 5) ok = 0;
    combat[0x48 / 4] = 0;
    {
        CDefPointer<const CMeleeCombatAbilityDef> z = c->GetPMeleeBlockCounterAbility();
        if (!z.IsNull()) ok = 0;
    }
    if (!ok) { printf("007334b0_TEST FAIL\n"); return 1; }
    printf("007334b0_TEST PASS\n");
    return 0;
}
