// Behaviour test for 0x00956850 CCombatAction_WillEnflameInto::IsToFaceTarget (NF427 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CCOMBATACTION_WILLENFLAMEINTO_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCombatAction_WillEnflameInto.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CCombatAction_WillEnflameInto)];
int main(void)
{
    const CCombatAction_WillEnflameInto* p = (const CCombatAction_WillEnflameInto*)g_obj;
    bool r = p->CCombatAction_WillEnflameInto::IsToFaceTarget();
    int ok = (r == true);
    if (ok) { printf("RB_956850_OK PASS\n"); return 0; }
    printf("RB_956850_OK FAIL\n");
    return 1;
}
