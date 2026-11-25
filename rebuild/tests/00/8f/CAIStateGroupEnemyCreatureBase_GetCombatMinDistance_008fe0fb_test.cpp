// Behaviour test for 0x008FE0FB CAIStateGroup_EnemyCreatureBase::GetCombatMinDistance (NF429 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#include "engine/CAIStateGroup_EnemyCreatureBase.h"
#include <stdio.h>
// GetCombatMinDistance is protected: a scratch accessor struct derived from CAIStateGroup_EnemyCreatureBase names it (no body of CAIStateGroup_EnemyCreatureBase, nothing instantiated).
struct NF429Access : CAIStateGroup_EnemyCreatureBase {
    static float call(const CAIStateGroup_EnemyCreatureBase* p) { return static_cast<const NF429Access*>(p)->CAIStateGroup_EnemyCreatureBase::GetCombatMinDistance(); }
};
static unsigned char g_obj[sizeof(CAIStateGroup_EnemyCreatureBase)];
int main(void)
{
    const CAIStateGroup_EnemyCreatureBase* p = (const CAIStateGroup_EnemyCreatureBase*)g_obj;
    float r = NF429Access::call(p);
    int ok = (r == 1.5f);
    if (ok) { printf("PASS_008fe0fb\n"); return 0; }
    printf("FAIL_008fe0fb\n");
    return 1;
}
