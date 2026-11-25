// Behaviour test for 0x00976FA0 CCreatureAction_JackShotFloat::IsToFaceTarget (NF427 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CCREATUREACTION_JACKSHOTFLOAT_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_JackShotFloat.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CCreatureAction_JackShotFloat)];
int main(void)
{
    const CCreatureAction_JackShotFloat* p = (const CCreatureAction_JackShotFloat*)g_obj;
    bool r = p->CCreatureAction_JackShotFloat::IsToFaceTarget();
    int ok = (r == true);
    if (ok) { printf("RB_976fa0_OK PASS\n"); return 0; }
    printf("RB_976fa0_OK FAIL\n");
    return 1;
}
