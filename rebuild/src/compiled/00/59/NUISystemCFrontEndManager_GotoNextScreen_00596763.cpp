// NUISystem::CFrontEndManager::GotoNextScreen(CComponent* pNextScreen, bool clearStack) -- push a new screen: close the current one, optionally clear the stack, push the new one, play the screen-change sound
// Leaf: ?GotoNextScreen@CFrontEndManager@NUISystem@@QAEXPAVCComponent@2@_N@Z
// Flags: none
// Retail: Fable.exe @ 0x00596763, 266 bytes, `ret 8`; Ego_r.pdb + FableWin.pdb same symbol (engine/CFrontEndManager.h VIRTUALKEYBOARD).
// Body: pCurrentScreen = m_stackScreens.top() (deque::back 0x0059B039); DeactivateAllEditBoxes(0) (0x005952D8); if pNextScreen is
//   m_mapScreens[FRONTEND_SCREEN_QUIT] (map::operator[] 0x0059B5D7) and PSoundManager exists: SetGainFader(0x100, 0.0f, 1000.0f)
//   (slot 46); pCurrentScreen->ChangeState(6) (CComponent slot 48); CManager::GetInstance() (0x0041E5F2)->RemoveObserver(pCurrentScreen)
//   (CObserver base +4, slot 5); if clearStack: while m_stackScreens.size() (the iterator difference _M_subtract 0x0059A8F3) pop()
//   (pop_back 0x0059AEE5); push(pNextScreen) (push_back 0x0059B61C); DeactivateAllEditBoxes(0); PScreenToActivate = pNextScreen;
//   PScreenToDeactivate = pCurrentScreen; CManager::GetInstance()->PlaySoundW(GetMiscThingsDef()->SoundForward, 1) (slots 40 / 42; the
//   CDefPointer temporary released inline). Parameter and local names as Ego_d.pdb (pNextScreen, clearStack, pCurrentScreen).
// NF79 (round 7, q10967): ported onto the shared headers (engine/CFrontEndManager.h full-layout branch with NF76's VIRTUALKEYBOARD opt-in
//   and my STACKSCREENS opt-in; stlport/fable_deque.h STLP_STACK_TOP_POP + my nested STLP_STACK_SIZE_PUSH + DEQUE_PUSH_BACK_OUTOFLINE;
//   NUISystem_CManager.h, NUISystem_CUIMiscThingsDef.h, CDefPointer.h, CASoundManager.h); was a local model of the deque / stack / map /
//   CManager / CComponent / CFrontEndManager that bound pop_back, back, push_back and the iterator difference under stand-in spellings.
// Opt: size TU (unaligned entry) with a frame pointer (`leave`).
#pragma optimize("s", on)
#pragma optimize("y", off)
#define FABLE_RETAIL_STLP_STACK_TOP_POP
#define FABLE_RETAIL_STLP_STACK_SIZE_PUSH
#define FABLE_RETAIL_STLP_DEQUE_PUSH_BACK_OUTOFLINE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CFRONTENDMANAGER_DESTRUCTOR
#define FABLE_RETAIL_CFRONTENDMANAGER_VIRTUALKEYBOARD
#define FABLE_RETAIL_CFRONTENDMANAGER_STACKSCREENS
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "stlport/fable_deque.h"
#include "fable_string.h"
#include "engine/CFrontEndManager.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CUIMiscThingsDef.h"
#include "engine/CDefPointer.h"
#include "engine/CASoundManager.h"

namespace NUISystem {
void CFrontEndManager::GotoNextScreen(CComponent* pNextScreen, bool clearStack)
{
    CComponent* pCurrentScreen = m_stackScreens.top();
    DeactivateAllEditBoxes(0);
    if (pNextScreen == m_mapScreens[FRONTEND_SCREEN_QUIT] && PSoundManager)
        PSoundManager->SetGainFader(0x100, 0.0f, 1000.0f);
    pCurrentScreen->ChangeState(6);
    CManager::GetInstance()->RemoveObserver(pCurrentScreen);
    if (clearStack)
        while (m_stackScreens.size())
            m_stackScreens.pop();
    m_stackScreens.push(pNextScreen);
    DeactivateAllEditBoxes(0);
    PScreenToActivate = pNextScreen;
    PScreenToDeactivate = pCurrentScreen;
    CManager::GetInstance()->PlaySoundW(CManager::GetInstance()->GetMiscThingsDef()->SoundForward, 1);
}
}
