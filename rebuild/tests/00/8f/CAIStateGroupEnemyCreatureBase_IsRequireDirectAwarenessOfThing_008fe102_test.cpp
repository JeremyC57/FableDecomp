// Behaviour test for 0x008FE102 CAIStateGroup_EnemyCreatureBase::IsRequireDirectAwarenessOfThing (NF429 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#include "engine/CAIStateGroup_EnemyCreatureBase.h"
#include <stdio.h>
// IsRequireDirectAwarenessOfThing is protected: a scratch accessor struct derived from CAIStateGroup_EnemyCreatureBase names it (no body of CAIStateGroup_EnemyCreatureBase, nothing instantiated).
struct NF429Access : CAIStateGroup_EnemyCreatureBase {
    static bool call(const CAIStateGroup_EnemyCreatureBase* p) { return static_cast<const NF429Access*>(p)->CAIStateGroup_EnemyCreatureBase::IsRequireDirectAwarenessOfThing(); }
};
static unsigned char g_obj[sizeof(CAIStateGroup_EnemyCreatureBase)];
int main(void)
{
    const CAIStateGroup_EnemyCreatureBase* p = (const CAIStateGroup_EnemyCreatureBase*)g_obj;
    bool r = NF429Access::call(p);
    int ok = (r == true);
    if (ok) { printf("AUTO_TINY_008fe102_TEST PASS\n"); return 0; }
    printf("FAIL_008fe102\n");
    return 1;
}
