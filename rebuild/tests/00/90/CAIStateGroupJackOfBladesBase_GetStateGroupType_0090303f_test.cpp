// Behaviour test for 0x0090303F CAIStateGroup_JackOfBladesBase::GetStateGroupType (NF429 G1): links the row object and calls the member directly (qualified call, no vftable);
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
    EAIStateGroupType r = p->CAIStateGroup_JackOfBladesBase::GetStateGroupType();
    int ok = (r == (EAIStateGroupType)1);
    if (ok) { printf("PASS_0090303f\n"); return 0; }
    printf("FAIL_0090303f\n");
    return 1;
}
