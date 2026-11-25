// Behaviour test for 0x006F5AB0 CCreatureAction_GenericAimProjectileWeapon::GetIsToFaceTargetCone (NF427 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CCREATUREACTION_GENERICAIMPROJECTILEWEAPON_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_GenericAimProjectileWeapon.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CCreatureAction_GenericAimProjectileWeapon)];
int main(void)
{
    const CCreatureAction_GenericAimProjectileWeapon* p = (const CCreatureAction_GenericAimProjectileWeapon*)g_obj;
    float r = p->CCreatureAction_GenericAimProjectileWeapon::GetIsToFaceTargetCone();
    int ok = (r == 1.0f);
    if (ok) { printf("PASS_006f5ab0 PASS\n"); return 0; }
    printf("PASS_006f5ab0 FAIL\n");
    return 1;
}
