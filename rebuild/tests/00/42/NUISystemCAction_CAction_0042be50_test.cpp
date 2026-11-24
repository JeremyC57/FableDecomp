// Behaviour test for 0x0042BE50 NUISystem::CAction::CAction() (NF229) on the shared header engine/NUISystem_CAction.h /
// stlport/fable_containers.h: a new action has ActionType UI_ACTION_TYPE_NONE, Valid true, and two empty parameter lists
// (UIActionParams, ThingActionParams: each list's sentinel node linked to itself, distinct per list and per action); the lists are usable.
// The constructor body is the row's (its member initialisers); no stub, no model. One case per condition.
#define FABLE_RETAIL_STLP_LIST
#include <stdio.h>
#include "engine/NUISystem_CAction.h"
namespace NUISystem {
CAction::CAction()
    : ActionType(UI_ACTION_TYPE_NONE), Valid(true)
{
}
}
static int g_fails = 0;
static void Check(bool ok, const char* what) { if (!ok) { printf("FAIL: %s\n", what); ++g_fails; } }
int main()
{
    {
        NUISystem::CAction a;
        NUISystem::CAction b;
        Check(a.ActionType == NUISystem::UI_ACTION_TYPE_NONE, "type none");
        Check(a.Valid, "valid");
        Check(a.UIActionParams.begin() == a.UIActionParams.end(), "UI parameter list empty");
        Check(a.ThingActionParams.begin() == a.ThingActionParams.end(), "thing parameter list empty");
        Check(!(a.UIActionParams.end() == b.UIActionParams.end()) && !(a.ThingActionParams.end() == b.ThingActionParams.end()), "each action has its own lists");
        a.ThingActionParams.push_back(7);
        Check(!(a.ThingActionParams.begin() == a.ThingActionParams.end()) && *a.ThingActionParams.begin() == 7, "usable: one element");
        Check(b.ThingActionParams.begin() == b.ThingActionParams.end(), "the other action untouched");
    }
    printf(g_fails ? "0042be50_TEST FAIL\n" : "0042be50_TEST PASS\n");
    return g_fails ? 1 : 0;
}
