// Behaviour test for 0x00842140 CCreatureAction_UseWallMount::Initialise (NF427 G2): links the row object, defines CCreatureActionBase::Initialise here (counts calls, records this),
// calls the member directly (qualified, no vftable) and checks exactly one forwarded call with the same object.
#define FABLE_RETAIL_CCREATUREACTION_USEWALLMOUNT_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "engine/CCreatureAction_UseWallMount.h"
#include <stdio.h>
static int g_calls = 0; static const void* g_this = 0;
void CCreatureActionBase::Initialise() { ++g_calls; g_this = this; }
static unsigned char g_obj[sizeof(CCreatureAction_UseWallMount)];
int main(void)
{
    CCreatureAction_UseWallMount* p = (CCreatureAction_UseWallMount*)g_obj;
    p->CCreatureAction_UseWallMount::Initialise();
    int ok = (g_calls == 1 && g_this == (const void*)static_cast<CCreatureActionBase*>(p));
    if (ok) { printf("PASS_00842140 PASS\n"); return 0; }
    printf("PASS_00842140 FAIL\n");
    return 1;
}
