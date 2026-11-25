// Behaviour test for 0x0085C300 CCombatAction_ControlledStrafeJump::GetIsToFaceTargetCone (NF427 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CCOMBATACTION_CONTROLLEDSTRAFEJUMP_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCombatAction_ControlledStrafeJump.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CCombatAction_ControlledStrafeJump)];
int main(void)
{
    const CCombatAction_ControlledStrafeJump* p = (const CCombatAction_ControlledStrafeJump*)g_obj;
    float r = p->CCombatAction_ControlledStrafeJump::GetIsToFaceTargetCone();
    int ok = (r == 0.99722224f);
    if (ok) { printf("PASS_0085c300 PASS\n"); return 0; }
    printf("PASS_0085c300 FAIL\n");
    return 1;
}
