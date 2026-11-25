// Behaviour test for 0x00900F20 CAIStateGroup_RangedCombatUsePreferredFirePoint::IsCombatStateGroup (NF429 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CAISTATEGROUP_RANGEDCOMBATUSEPREFERREDFIREPOINT_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#include "engine/CAIStateGroup_RangedCombatUsePreferredFirePoint.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CAIStateGroup_RangedCombatUsePreferredFirePoint)];
int main(void)
{
    const CAIStateGroup_RangedCombatUsePreferredFirePoint* p = (const CAIStateGroup_RangedCombatUsePreferredFirePoint*)g_obj;
    bool r = p->CAIStateGroup_RangedCombatUsePreferredFirePoint::IsCombatStateGroup();
    int ok = (r == true);
    if (ok) { printf("AUTO_TINY_00900f20_TEST PASS\n"); return 0; }
    printf("FAIL_00900f20\n");
    return 1;
}
