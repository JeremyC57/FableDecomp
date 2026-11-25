// Behaviour test for 0x00929640 CAIStateGroup_MinionGuardGreetPlayer::Initialise (NF429 G2): links the row object and calls the member directly (qualified call, no vftable);
// defines CAIStateGroupBase::Initialise here (counts calls, records this and the arguments), checks exactly one forwarded call with the same object and arguments. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CAISTATEGROUP_MINIONGUARDGREETPLAYER_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#include "engine/CAIStateGroup_MinionGuardGreetPlayer.h"
#include <stdio.h>
static unsigned char g_arg0[16];
static unsigned char g_arg1[16];
static unsigned char g_arg4[16];
static int g_calls = 0; static const void* g_this = 0; static int g_argok = 0;
void CAIStateGroupBase::Initialise(CAIBrain& brain, CThingAICreature& creature, long priority, EScriptAIPriority scriptPriority, const CCharString& name) { ++g_calls; g_this = this; g_argok = 1 && (const void*)&brain == (const void*)g_arg0 && (const void*)&creature == (const void*)g_arg1 && priority == (long)9 && scriptPriority == (EScriptAIPriority)10 && (const void*)&name == (const void*)g_arg4; }
static unsigned char g_obj[sizeof(CAIStateGroup_MinionGuardGreetPlayer)];
int main(void)
{
    CAIStateGroup_MinionGuardGreetPlayer* p = (CAIStateGroup_MinionGuardGreetPlayer*)g_obj;
    p->CAIStateGroup_MinionGuardGreetPlayer::Initialise(*(CAIBrain*)g_arg0, *(CThingAICreature*)g_arg1, (long)9, (EScriptAIPriority)10, *(CCharString*)g_arg4);
    int ok = (g_calls == 1 && g_this == (const void*)static_cast<CAIStateGroupBase*>(p) && g_argok);
    if (ok) { printf("PASS_00929640\n"); return 0; }
    printf("FAIL_00929640\n");
    return 1;
}
