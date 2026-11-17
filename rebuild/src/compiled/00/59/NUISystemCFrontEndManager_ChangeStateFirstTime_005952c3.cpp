// NUISystem::CFrontEndManager::ChangeStateFirstTime() -- put the screen on top of the screen stack into state 5
// Leaf: ?ChangeStateFirstTime@CFrontEndManager@NUISystem@@QAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x005952C3, 21 bytes: `add ecx,20h ; call 0x0059B039` (m_stackScreens.top(), the out-of-line
//   std::deque<NUISystem::CComponent*>::back), then the top component's slot 48 (+0xC0) ChangeState(unsigned long) with 5.
// Identity: Ego_r.pdb 0x00601D31 + FableWin.pdb 0x0201D44E ?ChangeStateFirstTime@CFrontEndManager@NUISystem@@QAEXXZ; the landed caller
//   0x0042EC7C CNewFrontendGameComponent::Run binds this spelling.
// NF79 (round 7, q10966): ported onto engine/CFrontEndManager.h's full-layout branch (NF76's 0x00599E3F macro block, STLP_STACK_TOP_POP for
//   the inline stack::top() over the out-of-line back) + my overlay opt-in CFRONTENDMANAGER_STACKSCREENS; was the free fastcall stand-in
//   CFrontEndManager_ChangeStateFirstTime_005952c3 with FableFrontEndFirstStateStackTop and a slot-prefix component model.
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_STACK_TOP_POP
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CFRONTENDMANAGER_DESTRUCTOR
#define FABLE_RETAIL_CFRONTENDMANAGER_STACKSCREENS
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "stlport/fable_deque.h"
#include "fable_string.h"
#include "engine/CFrontEndManager.h"

namespace NUISystem {
void CFrontEndManager::ChangeStateFirstTime()
{
    m_stackScreens.top()->ChangeState(5);
}
}
