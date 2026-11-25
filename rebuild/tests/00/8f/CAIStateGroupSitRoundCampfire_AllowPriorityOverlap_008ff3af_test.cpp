// Behaviour test for 0x008FF3AF CAIStateGroup_SitRoundCampfire::AllowPriorityOverlap (NF429 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CAISTATEGROUP_SITROUNDCAMPFIRE_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#include "engine/CAIStateGroup_SitRoundCampfire.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CAIStateGroup_SitRoundCampfire)];
int main(void)
{
    CAIStateGroup_SitRoundCampfire* p = (CAIStateGroup_SitRoundCampfire*)g_obj;
    bool r = p->CAIStateGroup_SitRoundCampfire::AllowPriorityOverlap();
    int ok = (r == true);
    if (ok) { printf("AUTO_TINY_008ff3af_TEST PASS\n"); return 0; }
    printf("FAIL_008ff3af\n");
    return 1;
}
