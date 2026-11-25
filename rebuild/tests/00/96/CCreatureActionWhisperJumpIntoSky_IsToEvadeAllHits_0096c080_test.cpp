// Behaviour test for 0x0096C080 CCreatureAction_WhisperJumpIntoSky::IsToEvadeAllHits (NF427 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CCREATUREACTION_WHISPERJUMPINTOSKY_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_WhisperJumpIntoSky.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CCreatureAction_WhisperJumpIntoSky)];
int main(void)
{
    const CCreatureAction_WhisperJumpIntoSky* p = (const CCreatureAction_WhisperJumpIntoSky*)g_obj;
    bool r = p->CCreatureAction_WhisperJumpIntoSky::IsToEvadeAllHits();
    int ok = (r == true);
    if (ok) { printf("RB_96c080_OK PASS\n"); return 0; }
    printf("RB_96c080_OK FAIL\n");
    return 1;
}
