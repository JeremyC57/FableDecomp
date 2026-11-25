// Behaviour test for 0x006F5AF0 CCreatureAction_GenericFireProjectileWeapon::GetIsToFaceTargetCone (NF427 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CCREATUREACTION_GENERICFIREPROJECTILEWEAPON_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_GenericFireProjectileWeapon.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CCreatureAction_GenericFireProjectileWeapon)];
int main(void)
{
    const CCreatureAction_GenericFireProjectileWeapon* p = (const CCreatureAction_GenericFireProjectileWeapon*)g_obj;
    float r = p->CCreatureAction_GenericFireProjectileWeapon::GetIsToFaceTargetCone();
    int ok = (r == 1.0f);
    if (ok) { printf("PASS_006f5af0 PASS\n"); return 0; }
    printf("PASS_006f5af0 FAIL\n");
    return 1;
}
