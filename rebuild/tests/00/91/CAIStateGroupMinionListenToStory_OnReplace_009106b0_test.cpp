// Behaviour test for 0x009106B0 CAIStateGroup_MinionListenToStory::OnReplace (NF429 G2b): links the row object and calls the member directly (qualified call, no vftable);
// defines CAIStateGroup_MinionListenToStory::StandUp here (counts calls, records this, returns a marker), checks exactly one forwarded call with the same object. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CAISTATEGROUP_MINIONLISTENTOSTORY_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#include "engine/CAIStateGroup_MinionListenToStory.h"
#include <stdio.h>
// OnReplace is protected: a scratch accessor struct derived from CAIStateGroup_MinionListenToStory names it (no body of CAIStateGroup_MinionListenToStory, nothing instantiated).
struct NF429Access : CAIStateGroup_MinionListenToStory {
    static void call(CAIStateGroup_MinionListenToStory* p) { return static_cast<NF429Access*>(p)->CAIStateGroup_MinionListenToStory::OnReplace(); }
};
static int g_calls = 0; static const void* g_this = 0;
void CAIStateGroup_MinionListenToStory::StandUp() { ++g_calls; g_this = this; }
static unsigned char g_obj[sizeof(CAIStateGroup_MinionListenToStory)];
int main(void)
{
    CAIStateGroup_MinionListenToStory* p = (CAIStateGroup_MinionListenToStory*)g_obj;
    NF429Access::call(p);
    int ok = (g_calls == 1 && g_this == (const void*)p);
    if (ok) { printf("PASS_009106b0\n"); return 0; }
    printf("FAIL_009106b0\n");
    return 1;
}
