// Behaviour test for 0x00977190 CCreatureAction_JackScream::IsToEvadeAllHits (NF427 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CCREATUREACTION_JACKSCREAM_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_JackScream.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CCreatureAction_JackScream)];
int main(void)
{
    const CCreatureAction_JackScream* p = (const CCreatureAction_JackScream*)g_obj;
    bool r = p->CCreatureAction_JackScream::IsToEvadeAllHits();
    int ok = (r == true);
    if (ok) { printf("AUTO_TINY_00977190_TEST PASS\n"); return 0; }
    printf("AUTO_TINY_00977190_TEST FAIL\n");
    return 1;
}
