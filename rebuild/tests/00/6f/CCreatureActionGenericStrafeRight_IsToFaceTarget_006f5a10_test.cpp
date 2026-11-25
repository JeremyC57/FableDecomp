// Behaviour test for 0x006F5A10 CCreatureAction_GenericStrafeRight::IsToFaceTarget (NF427 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CCREATUREACTION_GENERICSTRAFERIGHT_VIRTUAL_MODEL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_KRAKEN_ROAR
#include "engine/CCreatureAction_GenericStrafeRight.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CCreatureAction_GenericStrafeRight)];
int main(void)
{
    const CCreatureAction_GenericStrafeRight* p = (const CCreatureAction_GenericStrafeRight*)g_obj;
    bool r = p->CCreatureAction_GenericStrafeRight::IsToFaceTarget();
    int ok = (r == true);
    if (ok) { printf("AUTO_TINY_006f5a10_TEST PASS\n"); return 0; }
    printf("AUTO_TINY_006f5a10_TEST FAIL\n");
    return 1;
}
