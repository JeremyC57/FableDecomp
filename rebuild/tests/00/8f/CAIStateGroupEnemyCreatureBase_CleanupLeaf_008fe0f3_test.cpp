// Behaviour test for 0x008FE0F3 CAIStateGroup_EnemyCreatureBase::CleanupLeaf (NF429 G1): links the row object and calls the member directly (qualified call, no vftable);
// calls it (nothing to observe: the body is empty). The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#include "engine/CAIStateGroup_EnemyCreatureBase.h"
#include <stdio.h>
// CleanupLeaf is protected: a scratch accessor struct derived from CAIStateGroup_EnemyCreatureBase names it (no body of CAIStateGroup_EnemyCreatureBase, nothing instantiated).
struct NF429Access : CAIStateGroup_EnemyCreatureBase {
    static void call(CAIStateGroup_EnemyCreatureBase* p) { return static_cast<NF429Access*>(p)->CAIStateGroup_EnemyCreatureBase::CleanupLeaf(); }
};
static unsigned char g_obj[sizeof(CAIStateGroup_EnemyCreatureBase)];
int main(void)
{
    CAIStateGroup_EnemyCreatureBase* p = (CAIStateGroup_EnemyCreatureBase*)g_obj;
    NF429Access::call(p);
    int ok = 1;
    if (ok) { printf("AUTO_TINY_008fe0f3_TEST PASS\n"); return 0; }
    printf("FAIL_008fe0f3\n");
    return 1;
}
