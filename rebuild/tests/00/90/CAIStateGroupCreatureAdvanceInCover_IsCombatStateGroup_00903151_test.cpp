// Behaviour test for 0x00903151 CAIStateGroup_CreatureAdvanceInCover::IsCombatStateGroup (NF429 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREADVANCEINCOVER_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE_VIRTUAL_MODEL
#include "engine/CAIStateGroup_CreatureAdvanceInCover.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CAIStateGroup_CreatureAdvanceInCover)];
int main(void)
{
    const CAIStateGroup_CreatureAdvanceInCover* p = (const CAIStateGroup_CreatureAdvanceInCover*)g_obj;
    bool r = p->CAIStateGroup_CreatureAdvanceInCover::IsCombatStateGroup();
    int ok = (r == true);
    if (ok) { printf("AUTO_TINY_00903151_TEST PASS\n"); return 0; }
    printf("FAIL_00903151\n");
    return 1;
}
