// Behaviour test for 0x00903043 CAIStateGroup_JackOfBladesBase::IsTimingCritical (NF429 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CAISTATEGROUP_JACKOFBLADESBASE_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#include "engine/CAIStateGroup_JackOfBladesBase.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CAIStateGroup_JackOfBladesBase)];
int main(void)
{
    const CAIStateGroup_JackOfBladesBase* p = (const CAIStateGroup_JackOfBladesBase*)g_obj;
    bool r = p->CAIStateGroup_JackOfBladesBase::IsTimingCritical();
    int ok = (r == true);
    if (ok) { printf("AUTO_TINY_00903043_TEST PASS\n"); return 0; }
    printf("FAIL_00903043\n");
    return 1;
}
