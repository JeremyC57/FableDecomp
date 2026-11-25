// Behaviour test for 0x0074DC20 CCreatureAction_OnGenerate::IsToFaceTargetAtStart (NF427 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CCREATUREACTION_ONGENERATE_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_OnGenerate.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CCreatureAction_OnGenerate)];
int main(void)
{
    const CCreatureAction_OnGenerate* p = (const CCreatureAction_OnGenerate*)g_obj;
    bool r = p->CCreatureAction_OnGenerate::IsToFaceTargetAtStart();
    int ok = (r == true);
    if (ok) { printf("AUTO_TINY_0074dc20_TEST PASS\n"); return 0; }
    printf("AUTO_TINY_0074dc20_TEST FAIL\n");
    return 1;
}
