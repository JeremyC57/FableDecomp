// Behaviour test for 0x008FDC8E CAIStateGroup_LookAtInterestingThings::AllowPriorityOverlap (NF429 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CAISTATEGROUP_LOOKATINTERESTINGTHINGS_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#include "engine/CAIStateGroup_LookAtInterestingThings.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CAIStateGroup_LookAtInterestingThings)];
int main(void)
{
    CAIStateGroup_LookAtInterestingThings* p = (CAIStateGroup_LookAtInterestingThings*)g_obj;
    bool r = p->CAIStateGroup_LookAtInterestingThings::AllowPriorityOverlap();
    int ok = (r == true);
    if (ok) { printf("AUTO_TINY_008fdc8e_TEST PASS\n"); return 0; }
    printf("FAIL_008fdc8e\n");
    return 1;
}
