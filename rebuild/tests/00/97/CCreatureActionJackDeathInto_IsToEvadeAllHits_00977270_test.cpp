// Behaviour test for 0x00977270 CCreatureAction_JackDeathInto::IsToEvadeAllHits (NF427 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CCREATUREACTION_JACKDEATHINTO_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_JackDeathInto.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CCreatureAction_JackDeathInto)];
int main(void)
{
    const CCreatureAction_JackDeathInto* p = (const CCreatureAction_JackDeathInto*)g_obj;
    bool r = p->CCreatureAction_JackDeathInto::IsToEvadeAllHits();
    int ok = (r == true);
    if (ok) { printf("RB_977270_OK PASS\n"); return 0; }
    printf("RB_977270_OK FAIL\n");
    return 1;
}
