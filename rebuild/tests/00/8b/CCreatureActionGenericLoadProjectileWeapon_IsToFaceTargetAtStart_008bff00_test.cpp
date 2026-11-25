// Behaviour test for 0x008BFF00 CCreatureAction_GenericLoadProjectileWeapon::IsToFaceTargetAtStart (NF427 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CCREATUREACTION_GENERICLOADPROJECTILEWEAPON_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_GenericLoadProjectileWeapon.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CCreatureAction_GenericLoadProjectileWeapon)];
int main(void)
{
    const CCreatureAction_GenericLoadProjectileWeapon* p = (const CCreatureAction_GenericLoadProjectileWeapon*)g_obj;
    bool r = p->CCreatureAction_GenericLoadProjectileWeapon::IsToFaceTargetAtStart();
    int ok = (r == true);
    if (ok) { printf("AUTO_TINY_008bff00_TEST PASS\n"); return 0; }
    printf("AUTO_TINY_008bff00_TEST FAIL\n");
    return 1;
}
