// Behaviour test for 0x008550A0 CCreatureAction_SpiritHitResponse::IsOverrideableByActionOfSamePriority (NF427 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CCREATUREACTION_SPIRITHITRESPONSE_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCREATUREACTIONBASE
#include "engine/CCreatureAction_SpiritHitResponse.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CCreatureAction_SpiritHitResponse)];
int main(void)
{
    const CCreatureAction_SpiritHitResponse* p = (const CCreatureAction_SpiritHitResponse*)g_obj;
    bool r = p->CCreatureAction_SpiritHitResponse::IsOverrideableByActionOfSamePriority();
    int ok = (r == true);
    if (ok) { printf("AUTO_TINY_008550a0_TEST PASS\n"); return 0; }
    printf("AUTO_TINY_008550a0_TEST FAIL\n");
    return 1;
}
