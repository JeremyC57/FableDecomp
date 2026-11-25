// Behaviour test for 0x00902F9A CAIStateGroup_CreatureUseCoverBase::ProcessCoverThingList (NF429 G1): links the row object and calls the member directly (qualified call, no vftable);
// calls it (nothing to observe: the body is empty). The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE_VIRTUAL_MODEL
#include "engine/CAIStateGroup_CreatureUseCoverBase.h"
#include <stdio.h>
// ProcessCoverThingList is protected: a scratch accessor struct derived from CAIStateGroup_CreatureUseCoverBase names it (no body of CAIStateGroup_CreatureUseCoverBase, nothing instantiated).
struct NF429Access : CAIStateGroup_CreatureUseCoverBase {
    static void call(CAIStateGroup_CreatureUseCoverBase* p, std::vector<CThing*, std::allocator<CThing*> >& x0) { return static_cast<NF429Access*>(p)->CAIStateGroup_CreatureUseCoverBase::ProcessCoverThingList(x0); }
};
static unsigned char g_arg0[16];
static unsigned char g_obj[sizeof(CAIStateGroup_CreatureUseCoverBase)];
int main(void)
{
    CAIStateGroup_CreatureUseCoverBase* p = (CAIStateGroup_CreatureUseCoverBase*)g_obj;
    NF429Access::call(p, *(std::vector<CThing*, std::allocator<CThing*> >*)g_arg0);
    int ok = 1;
    if (ok) { printf("AUTO_TINY_00902f9a_TEST PASS\n"); return 0; }
    printf("FAIL_00902f9a\n");
    return 1;
}
