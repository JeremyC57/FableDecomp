// Behaviour test for 0x009770E0 CCreatureAction_JackShotFar::IsToFaceTarget (NF427 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CCREATUREACTION_JACKSHOTFAR_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_JackShotFar.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CCreatureAction_JackShotFar)];
int main(void)
{
    const CCreatureAction_JackShotFar* p = (const CCreatureAction_JackShotFar*)g_obj;
    bool r = p->CCreatureAction_JackShotFar::IsToFaceTarget();
    int ok = (r == true);
    if (ok) { printf("RB_9770e0_OK PASS\n"); return 0; }
    printf("RB_9770e0_OK FAIL\n");
    return 1;
}
