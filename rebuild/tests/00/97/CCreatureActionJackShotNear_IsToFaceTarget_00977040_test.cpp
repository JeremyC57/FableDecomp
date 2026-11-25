// Behaviour test for 0x00977040 CCreatureAction_JackShotNear::IsToFaceTarget (NF427 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CCREATUREACTION_JACKSHOTNEAR_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_JackShotNear.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CCreatureAction_JackShotNear)];
int main(void)
{
    const CCreatureAction_JackShotNear* p = (const CCreatureAction_JackShotNear*)g_obj;
    bool r = p->CCreatureAction_JackShotNear::IsToFaceTarget();
    int ok = (r == true);
    if (ok) { printf("RB_977040_OK PASS\n"); return 0; }
    printf("RB_977040_OK FAIL\n");
    return 1;
}
