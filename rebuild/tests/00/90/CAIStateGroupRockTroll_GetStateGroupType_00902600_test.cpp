// Behaviour test for 0x00902600 CAIStateGroup_RockTroll::GetStateGroupType (NF429 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CAISTATEGROUP_ROCKTROLL_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUP_TROLLBASE_VIRTUAL_MODEL
#include "engine/CAIStateGroup_RockTroll.h"
#include <stdio.h>
// GetStateGroupType is protected: a scratch accessor struct derived from CAIStateGroup_RockTroll names it (no body of CAIStateGroup_RockTroll, nothing instantiated).
struct NF429Access : CAIStateGroup_RockTroll {
    static EAIStateGroupType call(const CAIStateGroup_RockTroll* p) { return static_cast<const NF429Access*>(p)->CAIStateGroup_RockTroll::GetStateGroupType(); }
};
static unsigned char g_obj[sizeof(CAIStateGroup_RockTroll)];
int main(void)
{
    const CAIStateGroup_RockTroll* p = (const CAIStateGroup_RockTroll*)g_obj;
    EAIStateGroupType r = NF429Access::call(p);
    int ok = (r == (EAIStateGroupType)1);
    if (ok) { printf("PASS_00902600\n"); return 0; }
    printf("FAIL_00902600\n");
    return 1;
}
