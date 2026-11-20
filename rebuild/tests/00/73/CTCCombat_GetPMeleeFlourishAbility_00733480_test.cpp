// Behaviour test for 0x00733480 (links the REAL row object; NF138): CTCCombat::GetPMeleeFlourishAbility() const returns the usable
// ability at CurrentAbilityIndex modulo the list length (UsableAbilityList +0x38, CurrentAbilityIndex +0x44) with a reference added.
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
#define FABLE_RETAIL_CTCCOMBAT_GETPMELEEFLOURISHABILITY
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCCombat.h"
#include <stdio.h>
int main()
{
    unsigned long a[8] = { 0, 3, 0, 0, 0, 0, 0, 0 }, b[8] = { 0, 3, 0, 0, 0, 0, 0, 0 };   // pointees: IVRefCount +4
    unsigned long list[2] = { (unsigned long)a, (unsigned long)b };                       // vector<CDefPointer<...> > storage
    unsigned long combat[0x98 / 4];
    for (int i = 0; i < 0x98 / 4; ++i) combat[i] = 0;
    combat[0x38 / 4] = (unsigned long)list; combat[0x3c / 4] = (unsigned long)(list + 2); combat[0x40 / 4] = (unsigned long)(list + 2);
    combat[0x44 / 4] = 3;
    const CTCCombat* c = (const CTCCombat*)combat;
    int ok = 1;
    {
        CDefPointer<const CMeleeCombatAbilityDef> p = c->GetPMeleeFlourishAbility();
        if ((void*)p.InternalGetObject() != (void*)b || b[1] != 4 || a[1] != 3) ok = 0;
    }
    if (b[1] != 3) ok = 0;
    combat[0x44 / 4] = 4;
    {
        CDefPointer<const CMeleeCombatAbilityDef> q = c->GetPMeleeFlourishAbility();
        if ((void*)q.InternalGetObject() != (void*)a || a[1] != 4) ok = 0;
    }
    if (!ok) { printf("00733480_TEST FAIL\n"); return 1; }
    printf("00733480_TEST PASS\n");
    return 0;
}
