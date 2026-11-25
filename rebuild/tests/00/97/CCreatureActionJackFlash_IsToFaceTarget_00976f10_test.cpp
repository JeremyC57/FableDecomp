// Behaviour test for 0x00976F10 CCreatureAction_JackFlash::IsToFaceTarget (NF427 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CCREATUREACTION_JACKFLASH_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_JackFlash.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CCreatureAction_JackFlash)];
int main(void)
{
    const CCreatureAction_JackFlash* p = (const CCreatureAction_JackFlash*)g_obj;
    bool r = p->CCreatureAction_JackFlash::IsToFaceTarget();
    int ok = (r == true);
    if (ok) { printf("RB_976f10_OK PASS\n"); return 0; }
    printf("RB_976f10_OK FAIL\n");
    return 1;
}
