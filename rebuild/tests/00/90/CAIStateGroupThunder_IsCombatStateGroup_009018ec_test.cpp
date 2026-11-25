// Behaviour test for 0x009018EC CAIStateGroup_Thunder::IsCombatStateGroup (NF429 G1): links the row object and calls the member directly (qualified call, no vftable);
// the constant must come back. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CAISTATEGROUP_THUNDER_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#include "engine/CAIStateGroup_Thunder.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CAIStateGroup_Thunder)];
int main(void)
{
    const CAIStateGroup_Thunder* p = (const CAIStateGroup_Thunder*)g_obj;
    bool r = p->CAIStateGroup_Thunder::IsCombatStateGroup();
    int ok = (r == false);
    if (ok) { printf("AUTO_TINY_009018ec_TEST PASS\n"); return 0; }
    printf("FAIL_009018ec\n");
    return 1;
}
