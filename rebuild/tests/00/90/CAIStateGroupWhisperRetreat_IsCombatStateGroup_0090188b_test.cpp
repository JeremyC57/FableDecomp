// Behaviour test for 0x0090188B CAIStateGroup_WhisperRetreat::IsCombatStateGroup (NF429 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CAISTATEGROUP_WHISPERRETREAT_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE_VIRTUAL_MODEL
#include "engine/CAIStateGroup_WhisperRetreat.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CAIStateGroup_WhisperRetreat)];
int main(void)
{
    const CAIStateGroup_WhisperRetreat* p = (const CAIStateGroup_WhisperRetreat*)g_obj;
    bool r = p->CAIStateGroup_WhisperRetreat::IsCombatStateGroup();
    int ok = (r == false);
    if (ok) { printf("AUTO_TINY_0090188b_TEST PASS\n"); return 0; }
    printf("FAIL_0090188b\n");
    return 1;
}
