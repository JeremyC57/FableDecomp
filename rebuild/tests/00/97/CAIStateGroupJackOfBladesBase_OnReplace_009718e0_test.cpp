// Behaviour test for 0x009718E0 CAIStateGroup_JackOfBladesBase::OnReplace (NF429 G1): links the row object and calls the member directly (qualified call, no vftable);
// calls it (nothing to observe: the body is empty). The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CAISTATEGROUP_JACKOFBLADESBASE_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#include "engine/CAIStateGroup_JackOfBladesBase.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CAIStateGroup_JackOfBladesBase)];
int main(void)
{
    CAIStateGroup_JackOfBladesBase* p = (CAIStateGroup_JackOfBladesBase*)g_obj;
    p->CAIStateGroup_JackOfBladesBase::OnReplace();
    int ok = 1;
    if (ok) { printf("AUTO_TINY_009718e0_TEST PASS\n"); return 0; }
    printf("FAIL_009718e0\n");
    return 1;
}
