// Behaviour test for 0x00977760 CAIStateGroup_Guildmaster::OnReplace (NF429 G1): links the row object and calls the member directly (qualified call, no vftable);
// calls it (nothing to observe: the body is empty). The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CAISTATEGROUP_GUILDMASTER_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#include "engine/CAIStateGroup_Guildmaster.h"
#include <stdio.h>
static unsigned char g_obj[sizeof(CAIStateGroup_Guildmaster)];
int main(void)
{
    CAIStateGroup_Guildmaster* p = (CAIStateGroup_Guildmaster*)g_obj;
    p->CAIStateGroup_Guildmaster::OnReplace();
    int ok = 1;
    if (ok) { printf("AUTO_TINY_00977760_TEST PASS\n"); return 0; }
    printf("FAIL_00977760\n");
    return 1;
}
