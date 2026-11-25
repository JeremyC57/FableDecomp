// Behaviour test for 0x0090F460 CAIStateGroup_MinionHasInformation::IsRunnable (NF429 G2b): links the row object and calls the member directly (qualified call, no vftable);
// defines CAIStateGroup_MinionHasInformation::CreatureWantsPlayersAttention here (counts calls, records this, returns a marker), checks exactly one forwarded call with the same object and its result returned. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CAISTATEGROUP_MINIONHASINFORMATION_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#include "engine/CAIStateGroup_MinionHasInformation.h"
#include <stdio.h>
static int g_calls = 0; static const void* g_this = 0;
bool CAIStateGroup_MinionHasInformation::CreatureWantsPlayersAttention() const { ++g_calls; g_this = this; return true; }
static unsigned char g_obj[sizeof(CAIStateGroup_MinionHasInformation)];
int main(void)
{
    CAIStateGroup_MinionHasInformation* p = (CAIStateGroup_MinionHasInformation*)g_obj;
    bool r = p->CAIStateGroup_MinionHasInformation::IsRunnable();
    int ok = (g_calls == 1 && g_this == (const void*)p && r == true);
    if (ok) { printf("PASS_0090f460\n"); return 0; }
    printf("FAIL_0090f460\n");
    return 1;
}
