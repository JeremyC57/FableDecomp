// Behaviour test for 0x008BFFA0 CCreatureAction_BodgeSideStrike::IsToFaceTargetAtStart (NF427 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CCREATUREACTION_BODGESIDESTRIKE_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_BodgeSideStrike.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CCreatureAction_BodgeSideStrike)];
int main(void)
{
    const CCreatureAction_BodgeSideStrike* p = (const CCreatureAction_BodgeSideStrike*)g_obj;
    bool r = p->CCreatureAction_BodgeSideStrike::IsToFaceTargetAtStart();
    int ok = (r == true);
    if (ok) { printf("AUTO_TINY_008bffa0_TEST PASS\n"); return 0; }
    printf("AUTO_TINY_008bffa0_TEST FAIL\n");
    return 1;
}
