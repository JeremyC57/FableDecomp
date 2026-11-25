// Behaviour test for 0x00953590 CCreatureAction_DragonHoverOutOf::IsMovementUsingPhysics (NF427 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CCREATUREACTION_DRAGONHOVEROUTOF_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_DragonHoverOutOf.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CCreatureAction_DragonHoverOutOf)];
int main(void)
{
    const CCreatureAction_DragonHoverOutOf* p = (const CCreatureAction_DragonHoverOutOf*)g_obj;
    bool r = p->CCreatureAction_DragonHoverOutOf::IsMovementUsingPhysics();
    int ok = (r == false);
    if (ok) { printf("RB_953590_OK PASS\n"); return 0; }
    printf("RB_953590_OK FAIL\n");
    return 1;
}
