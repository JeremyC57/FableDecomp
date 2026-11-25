// Behaviour test for 0x00953240 CCreatureAction_SnapDragon::IsToFaceMovementDirection (NF427 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CCREATUREACTION_SNAPDRAGON_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_SnapDragon.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CCreatureAction_SnapDragon)];
int main(void)
{
    const CCreatureAction_SnapDragon* p = (const CCreatureAction_SnapDragon*)g_obj;
    bool r = p->CCreatureAction_SnapDragon::IsToFaceMovementDirection();
    int ok = (r == false);
    if (ok) { printf("RB_953240_OK PASS\n"); return 0; }
    printf("RB_953240_OK FAIL\n");
    return 1;
}
