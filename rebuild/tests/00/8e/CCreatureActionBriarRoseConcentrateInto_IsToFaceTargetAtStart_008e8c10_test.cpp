// Behaviour test for 0x008E8C10 CCreatureAction_BriarRoseConcentrateInto::IsToFaceTargetAtStart (NF427 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CCREATUREACTION_BRIARROSECONCENTRATEINTO_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_BriarRoseConcentrateInto.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CCreatureAction_BriarRoseConcentrateInto)];
int main(void)
{
    const CCreatureAction_BriarRoseConcentrateInto* p = (const CCreatureAction_BriarRoseConcentrateInto*)g_obj;
    bool r = p->CCreatureAction_BriarRoseConcentrateInto::IsToFaceTargetAtStart();
    int ok = (r == true);
    if (ok) { printf("AUTO_TINY_008e8c10_TEST PASS\n"); return 0; }
    printf("AUTO_TINY_008e8c10_TEST FAIL\n");
    return 1;
}
