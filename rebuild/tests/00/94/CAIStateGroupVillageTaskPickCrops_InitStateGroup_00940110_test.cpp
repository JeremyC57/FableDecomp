// Behaviour test for 0x00940110 CAIStateGroup_VillageTaskPickCrops::InitStateGroup (NF429 G1): links the row object and calls the member directly (qualified call, no vftable);
// calls it (nothing to observe: the body is empty). The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CAISTATEGROUP_VILLAGETASKPICKCROPS_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#include "engine/CAIStateGroup_VillageTaskPickCrops.h"
#include <stdio.h>
static unsigned char g_arg0[16];
static unsigned char g_arg1[16];
static unsigned char g_obj[sizeof(CAIStateGroup_VillageTaskPickCrops)];
int main(void)
{
    CAIStateGroup_VillageTaskPickCrops* p = (CAIStateGroup_VillageTaskPickCrops*)g_obj;
    p->CAIStateGroup_VillageTaskPickCrops::InitStateGroup(*(CWorld*)g_arg0, *(CThingAICreature*)g_arg1);
    int ok = 1;
    if (ok) { printf("AUTO_TINY_00940110_TEST PASS\n"); return 0; }
    printf("FAIL_00940110\n");
    return 1;
}
