// Behaviour test for 0x009021DA CAIStateGroup_MinionSitOutside::AllowPriorityOverlap (NF429 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CAISTATEGROUP_MINIONSITOUTSIDE_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUP_USESSEAT_VIRTUALS_PASS27
#define FABLE_RETAIL_CAISTATEGROUP_MINIONSITONSEATBASE_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUP_MINIONSITONSEAT_VIRTUAL_MODEL
#include "engine/CAIStateGroup_MinionSitOutside.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CAIStateGroup_MinionSitOutside)];
int main(void)
{
    CAIStateGroup_MinionSitOutside* p = (CAIStateGroup_MinionSitOutside*)g_obj;
    bool r = p->CAIStateGroup_MinionSitOutside::AllowPriorityOverlap();
    int ok = (r == true);
    if (ok) { printf("AUTO_TINY_009021da_TEST PASS\n"); return 0; }
    printf("FAIL_009021da\n");
    return 1;
}
