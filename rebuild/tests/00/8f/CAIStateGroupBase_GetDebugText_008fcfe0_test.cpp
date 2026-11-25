// Behaviour test for 0x008FCFE0 CAIStateGroupBase::GetDebugText (NF429 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#include "engine/CAIStateGroupBase.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CAIStateGroupBase)];
int main(void)
{
    const CAIStateGroupBase* p = (const CAIStateGroupBase*)g_obj;
    const CCharString* r = p->CAIStateGroupBase::GetDebugText();
    int ok = (r == 0);
    if (ok) { printf("CAIStateGroupBase_008fcfe0_TEST PASS\n"); return 0; }
    printf("FAIL_008fcfe0\n");
    return 1;
}
