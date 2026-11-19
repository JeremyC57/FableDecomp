// NUISystem::CList::AddActionOnInactive(CCountedPointer<CAction>& action) -- probe
// Leaf: ?AddActionOnInactive@CList@NUISystem@@UAEXAAV?$CCountedPointer@VCAction@NUISystem@@@@@Z
#include "fable_compiler.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CLIST
#define FABLE_RETAIL_STLP_LIST_PUSHBACK_NOINLINE
#define FABLE_RETAIL_STLP_LIST_BASE_OUTOFLINE
#include "engine/NUISystem_CList.h"

void NUISystem::CList::AddActionOnInactive(CCountedPointer<NUISystem::CAction>& action)
{
    if (!PActionsOnInactive)
        PActionsOnInactive = new std::list<CCountedPointer<NUISystem::CAction> >;
    PActionsOnInactive->push_back(action);
}
