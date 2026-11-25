// Behaviour test for 0x00868310 CCombatAction_GenericStrikeResponseKnockdown::IsToAlignWithGroundNormal (NF427 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CCOMBATACTION_GENERICSTRIKERESPONSEKNOCKDOWN_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCombatAction_GenericStrikeResponseKnockdown.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CCombatAction_GenericStrikeResponseKnockdown)];
int main(void)
{
    const CCombatAction_GenericStrikeResponseKnockdown* p = (const CCombatAction_GenericStrikeResponseKnockdown*)g_obj;
    bool r = p->CCombatAction_GenericStrikeResponseKnockdown::IsToAlignWithGroundNormal();
    int ok = (r == true);
    if (ok) { printf("AUTO_TINY_00868310_TEST PASS\n"); return 0; }
    printf("AUTO_TINY_00868310_TEST FAIL\n");
    return 1;
}
