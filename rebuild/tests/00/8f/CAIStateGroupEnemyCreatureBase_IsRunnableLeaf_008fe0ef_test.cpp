// Behaviour test for 0x008FE0EF CAIStateGroup_EnemyCreatureBase::IsRunnableLeaf (NF429 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#include "engine/CAIStateGroup_EnemyCreatureBase.h"
#include <stdio.h>
// IsRunnableLeaf is protected: a scratch accessor struct derived from CAIStateGroup_EnemyCreatureBase names it (no body of CAIStateGroup_EnemyCreatureBase, nothing instantiated).
struct NF429Access : CAIStateGroup_EnemyCreatureBase {
    static bool call(CAIStateGroup_EnemyCreatureBase* p) { return static_cast<NF429Access*>(p)->CAIStateGroup_EnemyCreatureBase::IsRunnableLeaf(); }
};
static unsigned char g_obj[sizeof(CAIStateGroup_EnemyCreatureBase)];
int main(void)
{
    CAIStateGroup_EnemyCreatureBase* p = (CAIStateGroup_EnemyCreatureBase*)g_obj;
    bool r = NF429Access::call(p);
    int ok = (r == true);
    if (ok) { printf("AUTO_TINY_008fe0ef_TEST PASS\n"); return 0; }
    printf("FAIL_008fe0ef\n");
    return 1;
}
