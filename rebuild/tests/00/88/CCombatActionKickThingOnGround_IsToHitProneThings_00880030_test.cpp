// Behaviour test for 0x00880030 CCombatAction_KickThingOnGround::IsToHitProneThings (NF427 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CCOMBATACTION_KICKTHINGONGROUND_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCombatAction_KickThingOnGround.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CCombatAction_KickThingOnGround)];
int main(void)
{
    const CCombatAction_KickThingOnGround* p = (const CCombatAction_KickThingOnGround*)g_obj;
    bool r = p->CCombatAction_KickThingOnGround::IsToHitProneThings();
    int ok = (r == true);
    if (ok) { printf("AUTO_TINY_00880030_TEST PASS\n"); return 0; }
    printf("AUTO_TINY_00880030_TEST FAIL\n");
    return 1;
}
