// Behaviour test for 0x0097CAF0 CAIStateGroup_CreatureFindCover::InitStateGroup (NF429 G1): links the row object and calls the member directly (qualified call, no vftable);
// calls it (nothing to observe: the body is empty). The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREFINDCOVER_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE_VIRTUAL_MODEL
#include "engine/CAIStateGroup_CreatureFindCover.h"
#include <stdio.h>
static unsigned char g_arg0[16];
static unsigned char g_arg1[16];
static unsigned char g_obj[sizeof(CAIStateGroup_CreatureFindCover)];
int main(void)
{
    CAIStateGroup_CreatureFindCover* p = (CAIStateGroup_CreatureFindCover*)g_obj;
    p->CAIStateGroup_CreatureFindCover::InitStateGroup(*(CWorld*)g_arg0, *(CThingAICreature*)g_arg1);
    int ok = 1;
    if (ok) { printf("AUTO_TINY_0097caf0_TEST PASS\n"); return 0; }
    printf("FAIL_0097caf0\n");
    return 1;
}
