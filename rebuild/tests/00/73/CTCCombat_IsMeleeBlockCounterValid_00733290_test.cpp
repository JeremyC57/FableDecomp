// Behaviour test for 0x00733290 (links the REAL row object; NF138): CTCCombat::IsMeleeBlockCounterValid() const answers false
// whatever the component holds.
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
#define FABLE_RETAIL_CTCCOMBAT_ISMELEEBLOCKCOUNTERVALID
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCCombat.h"
#include <stdio.h>
int main()
{
    unsigned long combat[0x98 / 4];
    for (int i = 0; i < 0x98 / 4; ++i) combat[i] = 0xFFFFFFFF;
    const CTCCombat* c = (const CTCCombat*)combat;
    if (c->IsMeleeBlockCounterValid()) { printf("AUTO_TINY_00733290_TEST FAIL\n"); return 1; }
    printf("AUTO_TINY_00733290_TEST PASS\n");
    return 0;
}
