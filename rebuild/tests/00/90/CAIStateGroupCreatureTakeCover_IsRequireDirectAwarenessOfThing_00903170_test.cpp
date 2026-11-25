// Behaviour test for 0x00903170 CAIStateGroup_CreatureTakeCover::IsRequireDirectAwarenessOfThing (NF429 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CAISTATEGROUP_CREATURETAKECOVER_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE_VIRTUAL_MODEL
#include "engine/CAIStateGroup_CreatureTakeCover.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CAIStateGroup_CreatureTakeCover)];
int main(void)
{
    const CAIStateGroup_CreatureTakeCover* p = (const CAIStateGroup_CreatureTakeCover*)g_obj;
    bool r = p->CAIStateGroup_CreatureTakeCover::IsRequireDirectAwarenessOfThing();
    int ok = (r == false);
    if (ok) { printf("AUTO_TINY_00903170_TEST PASS\n"); return 0; }
    printf("FAIL_00903170\n");
    return 1;
}
