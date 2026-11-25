// Behaviour test for 0x008E4A40 CCreatureAction_SnapDragonOutOf::IsToFaceMovementDirection (NF427 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CCREATUREACTION_SNAPDRAGONOUTOF_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_SnapDragonOutOf.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CCreatureAction_SnapDragonOutOf)];
int main(void)
{
    const CCreatureAction_SnapDragonOutOf* p = (const CCreatureAction_SnapDragonOutOf*)g_obj;
    bool r = p->CCreatureAction_SnapDragonOutOf::IsToFaceMovementDirection();
    int ok = (r == false);
    if (ok) { printf("RB_8e4a40_OK PASS\n"); return 0; }
    printf("RB_8e4a40_OK FAIL\n");
    return 1;
}
