// Behaviour test for 0x00979900 CAIStateGroup_CreatureTakeCover::InitStateGroup (NF429 G1): links the row object and calls the member directly (qualified call, no vftable);
// calls it (nothing to observe: the body is empty). The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CAISTATEGROUP_CREATURETAKECOVER_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE_VIRTUAL_MODEL
#include "engine/CAIStateGroup_CreatureTakeCover.h"
#include <stdio.h>
static unsigned char g_arg0[16];
static unsigned char g_arg1[16];
static unsigned char g_obj[sizeof(CAIStateGroup_CreatureTakeCover)];
int main(void)
{
    CAIStateGroup_CreatureTakeCover* p = (CAIStateGroup_CreatureTakeCover*)g_obj;
    p->CAIStateGroup_CreatureTakeCover::InitStateGroup(*(CWorld*)g_arg0, *(CThingAICreature*)g_arg1);
    int ok = 1;
    if (ok) { printf("AUTO_TINY_00979900_TEST PASS\n"); return 0; }
    printf("FAIL_00979900\n");
    return 1;
}
