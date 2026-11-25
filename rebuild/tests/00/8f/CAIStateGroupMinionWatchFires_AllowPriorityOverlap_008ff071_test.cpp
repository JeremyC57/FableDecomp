// Behaviour test for 0x008FF071 CAIStateGroup_MinionWatchFires::AllowPriorityOverlap (NF429 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CAISTATEGROUP_MINIONWATCHFIRES_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#include "engine/CAIStateGroup_MinionWatchFires.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CAIStateGroup_MinionWatchFires)];
int main(void)
{
    CAIStateGroup_MinionWatchFires* p = (CAIStateGroup_MinionWatchFires*)g_obj;
    bool r = p->CAIStateGroup_MinionWatchFires::AllowPriorityOverlap();
    int ok = (r == true);
    if (ok) { printf("AUTO_TINY_008ff071_TEST PASS\n"); return 0; }
    printf("FAIL_008ff071\n");
    return 1;
}
