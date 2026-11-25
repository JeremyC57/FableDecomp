// Behaviour test for 0x0097C9A0 CCreatureAction_ScorpionPrepareTailPlunge::IsToEvadeAllHits (NF427 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CCREATUREACTION_SCORPIONPREPARETAILPLUNGE_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_ScorpionPrepareTailPlunge.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CCreatureAction_ScorpionPrepareTailPlunge)];
int main(void)
{
    const CCreatureAction_ScorpionPrepareTailPlunge* p = (const CCreatureAction_ScorpionPrepareTailPlunge*)g_obj;
    bool r = p->CCreatureAction_ScorpionPrepareTailPlunge::IsToEvadeAllHits();
    int ok = (r == true);
    if (ok) { printf("RB_97c9a0_OK PASS\n"); return 0; }
    printf("RB_97c9a0_OK FAIL\n");
    return 1;
}
