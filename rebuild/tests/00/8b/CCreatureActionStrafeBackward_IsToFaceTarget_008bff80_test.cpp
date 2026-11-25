// Behaviour test for 0x008BFF80 CCreatureAction_StrafeBackward::IsToFaceTarget (NF427 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CCREATUREACTION_STRAFEBACKWARD_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_StrafeBackward.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CCreatureAction_StrafeBackward)];
int main(void)
{
    const CCreatureAction_StrafeBackward* p = (const CCreatureAction_StrafeBackward*)g_obj;
    bool r = p->CCreatureAction_StrafeBackward::IsToFaceTarget();
    int ok = (r == true);
    if (ok) { printf("AUTO_TINY_008bff80_TEST PASS\n"); return 0; }
    printf("AUTO_TINY_008bff80_TEST FAIL\n");
    return 1;
}
