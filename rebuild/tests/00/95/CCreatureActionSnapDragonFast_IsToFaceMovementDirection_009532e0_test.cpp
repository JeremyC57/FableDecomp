// Behaviour test for 0x009532E0 CCreatureAction_SnapDragonFast::IsToFaceMovementDirection (NF427 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CCREATUREACTION_SNAPDRAGONFAST_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_SnapDragonFast.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CCreatureAction_SnapDragonFast)];
int main(void)
{
    const CCreatureAction_SnapDragonFast* p = (const CCreatureAction_SnapDragonFast*)g_obj;
    bool r = p->CCreatureAction_SnapDragonFast::IsToFaceMovementDirection();
    int ok = (r == false);
    if (ok) { printf("RB_9532e0_OK PASS\n"); return 0; }
    printf("RB_9532e0_OK FAIL\n");
    return 1;
}
