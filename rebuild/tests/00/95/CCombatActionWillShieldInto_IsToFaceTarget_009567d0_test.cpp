// Behaviour test for 0x009567D0 CCombatAction_WillShieldInto::IsToFaceTarget (NF427 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CCOMBATACTION_WILLSHIELDINTO_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCombatAction_WillShieldInto.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CCombatAction_WillShieldInto)];
int main(void)
{
    const CCombatAction_WillShieldInto* p = (const CCombatAction_WillShieldInto*)g_obj;
    bool r = p->CCombatAction_WillShieldInto::IsToFaceTarget();
    int ok = (r == true);
    if (ok) { printf("RB_9567d0_OK PASS\n"); return 0; }
    printf("RB_9567d0_OK FAIL\n");
    return 1;
}
