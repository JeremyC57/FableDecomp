// Behaviour test for 0x008FF26B CAIStateGroup_MinionSitOnSeatBase::IsStateGroupStillValid (NF429 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CAISTATEGROUP_MINIONSITONSEATBASE_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUP_USESSEAT_VIRTUALS_PASS27
#include "engine/CAIStateGroup_MinionSitOnSeatBase.h"
#include <stdio.h>
// IsStateGroupStillValid is protected: a scratch accessor struct derived from CAIStateGroup_MinionSitOnSeatBase names it (no body of CAIStateGroup_MinionSitOnSeatBase, nothing instantiated).
struct NF429Access : CAIStateGroup_MinionSitOnSeatBase {
    static bool call(const CAIStateGroup_MinionSitOnSeatBase* p) { return static_cast<const NF429Access*>(p)->CAIStateGroup_MinionSitOnSeatBase::IsStateGroupStillValid(); }
};
static unsigned char g_obj[sizeof(CAIStateGroup_MinionSitOnSeatBase)];
int main(void)
{
    const CAIStateGroup_MinionSitOnSeatBase* p = (const CAIStateGroup_MinionSitOnSeatBase*)g_obj;
    bool r = NF429Access::call(p);
    int ok = (r == true);
    if (ok) { printf("AUTO_TINY_008ff26b_TEST PASS\n"); return 0; }
    printf("FAIL_008ff26b\n");
    return 1;
}
