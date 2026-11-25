// Behaviour test for 0x00968190 CAIStateGroup_TavernKeeper::OnActivate (NF429 G1): links the row object and calls the member directly (qualified call, no vftable);
// calls it (nothing to observe: the body is empty). The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CAISTATEGROUP_TAVERNKEEPER_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#include "engine/CAIStateGroup_TavernKeeper.h"
#include <stdio.h>
// OnActivate is protected: a scratch accessor struct derived from CAIStateGroup_TavernKeeper names it (no body of CAIStateGroup_TavernKeeper, nothing instantiated).
struct NF429Access : CAIStateGroup_TavernKeeper {
    static void call(CAIStateGroup_TavernKeeper* p) { return static_cast<NF429Access*>(p)->CAIStateGroup_TavernKeeper::OnActivate(); }
};
static unsigned char g_obj[sizeof(CAIStateGroup_TavernKeeper)];
int main(void)
{
    CAIStateGroup_TavernKeeper* p = (CAIStateGroup_TavernKeeper*)g_obj;
    NF429Access::call(p);
    int ok = 1;
    if (ok) { printf("AUTO_TINY_00968190_TEST PASS\n"); return 0; }
    printf("FAIL_00968190\n");
    return 1;
}
