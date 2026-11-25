// Behaviour test for 0x006F5670 CCreatureAction_ScreamerBackOff::IsToFaceTarget (NF427 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CCREATUREACTION_SCREAMERBACKOFF_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#define FABLE_RETAIL_CCREATUREACTION_STRAFEBACKWARD_VIRTUAL_MODEL
#include "engine/CCreatureAction_ScreamerBackOff.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CCreatureAction_ScreamerBackOff)];
int main(void)
{
    const CCreatureAction_ScreamerBackOff* p = (const CCreatureAction_ScreamerBackOff*)g_obj;
    bool r = p->CCreatureAction_ScreamerBackOff::IsToFaceTarget();
    int ok = (r == true);
    if (ok) { printf("AUTO_TINY_006f5670_TEST PASS\n"); return 0; }
    printf("AUTO_TINY_006f5670_TEST FAIL\n");
    return 1;
}
