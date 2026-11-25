// Behaviour test for 0x009031F0 CAIStateGroup_CreatureFindCover::IsCombatStateGroup (NF429 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREFINDCOVER_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE_VIRTUAL_MODEL
#include "engine/CAIStateGroup_CreatureFindCover.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CAIStateGroup_CreatureFindCover)];
int main(void)
{
    const CAIStateGroup_CreatureFindCover* p = (const CAIStateGroup_CreatureFindCover*)g_obj;
    bool r = p->CAIStateGroup_CreatureFindCover::IsCombatStateGroup();
    int ok = (r == true);
    if (ok) { printf("AUTO_TINY_009031f0_TEST PASS\n"); return 0; }
    printf("FAIL_009031f0\n");
    return 1;
}
