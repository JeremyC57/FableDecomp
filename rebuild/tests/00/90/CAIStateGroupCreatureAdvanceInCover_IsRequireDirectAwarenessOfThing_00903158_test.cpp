// Behaviour test for 0x00903158 CAIStateGroup_CreatureAdvanceInCover::IsRequireDirectAwarenessOfThing (NF429 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREADVANCEINCOVER_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE_VIRTUAL_MODEL
#include "engine/CAIStateGroup_CreatureAdvanceInCover.h"
#include <stdio.h>
// IsRequireDirectAwarenessOfThing is protected: a scratch accessor struct derived from CAIStateGroup_CreatureAdvanceInCover names it (no body of CAIStateGroup_CreatureAdvanceInCover, nothing instantiated).
struct NF429Access : CAIStateGroup_CreatureAdvanceInCover {
    static bool call(const CAIStateGroup_CreatureAdvanceInCover* p) { return static_cast<const NF429Access*>(p)->CAIStateGroup_CreatureAdvanceInCover::IsRequireDirectAwarenessOfThing(); }
};
static unsigned char g_obj[sizeof(CAIStateGroup_CreatureAdvanceInCover)];
int main(void)
{
    const CAIStateGroup_CreatureAdvanceInCover* p = (const CAIStateGroup_CreatureAdvanceInCover*)g_obj;
    bool r = NF429Access::call(p);
    int ok = (r == false);
    if (ok) { printf("AUTO_TINY_00903158_TEST PASS\n"); return 0; }
    printf("FAIL_00903158\n");
    return 1;
}
