// Behaviour test for 0x00933DA0 CAIStateGroup_MinionKidAnnoy::IsRunnable (NF429 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CAISTATEGROUP_MINIONKIDANNOY_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#include "engine/CAIStateGroup_MinionKidAnnoy.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CAIStateGroup_MinionKidAnnoy)];
int main(void)
{
    CAIStateGroup_MinionKidAnnoy* p = (CAIStateGroup_MinionKidAnnoy*)g_obj;
    bool r = p->CAIStateGroup_MinionKidAnnoy::IsRunnable();
    int ok = (r == false);
    if (ok) { printf("AUTO_TINY_00933da0_TEST PASS\n"); return 0; }
    printf("FAIL_00933da0\n");
    return 1;
}
