// Behaviour test for 0x00961AA0 CAIStateGroup_IceTrollIcicleAssault::Initialise (NF429 G2): links the row object and calls the member directly (qualified call, no vftable);
// defines CAIStateGroupBase::Initialise here (counts calls, records this and the arguments), checks exactly one forwarded call with the same object and arguments. The object image is static storage: the member reads nothing.
#define FABLE_RETAIL_CAISTATEGROUP_ICETROLLICICLEASSAULT_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#include "engine/CAIStateGroup_IceTrollIcicleAssault.h"
#include <stdio.h>
// Initialise is protected: a scratch accessor struct derived from CAIStateGroup_IceTrollIcicleAssault names it (no body of CAIStateGroup_IceTrollIcicleAssault, nothing instantiated).
struct NF429Access : CAIStateGroup_IceTrollIcicleAssault {
    static void call(CAIStateGroup_IceTrollIcicleAssault* p, CAIBrain& x0, CThingAICreature& x1, long x2, EScriptAIPriority x3, const CCharString& x4) { return static_cast<NF429Access*>(p)->CAIStateGroup_IceTrollIcicleAssault::Initialise(x0, x1, x2, x3, x4); }
};
static unsigned char g_arg0[16];
static unsigned char g_arg1[16];
static unsigned char g_arg4[16];
static int g_calls = 0; static const void* g_this = 0; static int g_argok = 0;
void CAIStateGroupBase::Initialise(CAIBrain& brain, CThingAICreature& creature, long priority, EScriptAIPriority scriptPriority, const CCharString& name) { ++g_calls; g_this = this; g_argok = 1 && (const void*)&brain == (const void*)g_arg0 && (const void*)&creature == (const void*)g_arg1 && priority == (long)9 && scriptPriority == (EScriptAIPriority)10 && (const void*)&name == (const void*)g_arg4; }
static unsigned char g_obj[sizeof(CAIStateGroup_IceTrollIcicleAssault)];
int main(void)
{
    CAIStateGroup_IceTrollIcicleAssault* p = (CAIStateGroup_IceTrollIcicleAssault*)g_obj;
    NF429Access::call(p, *(CAIBrain*)g_arg0, *(CThingAICreature*)g_arg1, (long)9, (EScriptAIPriority)10, *(CCharString*)g_arg4);
    int ok = (g_calls == 1 && g_this == (const void*)static_cast<CAIStateGroupBase*>(p) && g_argok);
    if (ok) { printf("PASS_00961aa0\n"); return 0; }
    printf("FAIL_00961aa0\n");
    return 1;
}
