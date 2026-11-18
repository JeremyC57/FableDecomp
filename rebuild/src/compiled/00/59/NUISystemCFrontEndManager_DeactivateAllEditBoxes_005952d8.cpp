// NUISystem::CFrontEndManager::DeactivateAllEditBoxes(CComponent* pParent) -- recursively deactivate every edit box under a component (none given: the top screen)
// Leaf: ?DeactivateAllEditBoxes@CFrontEndManager@NUISystem@@QAEXPAVCComponent@2@@Z
// Flags: none
// Retail: Fable.exe @ 0x005952D8, 126 bytes, `ret 4`; Ego_r.pdb 0x00602126 + FableWin.pdb 0x0201E772 same symbol.
// Body: if !pParent: pParent = m_stackScreens.top() (std::deque<NUISystem::CComponent*>::back 0x0059B039, out of line), and return if
//   that is null. If pParent->GetType() (slot 65) == UI_TYPE_EDIT_BOX (0x25) the edit box gets SetActive(false) (CEditBox slot 150).
//   Then, for i < GetChildren().size() (slot 53, re-read every pass), recurse on the child's raw pointer (8-byte CCountedPointer elements).
//   Parameter and locals as Ego_d.pdb names them (pParent, pEditBox in the if block, unsigned long i in the loop block).
// NF79 (round 7, q10966): ported onto the shared headers (engine/CFrontEndManager.h full-layout branch + my overlay opt-in
//   CFRONTENDMANAGER_STACKSCREENS, STLP_STACK_TOP_POP, engine/NUISystem_CEditBox.h); was a local model of CFrontEndManager / CComponent /
//   CEditBox / std::stack / std::deque / std::vector that bound back() as the allocator-less deque spelling.
// Opt: size TU (unaligned entry).
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
#include "engine/NUISystem_CEditBox.h"

namespace NUISystem {
void CFrontEndManager::DeactivateAllEditBoxes(CComponent* pParent)
{
    if (!pParent) {
        pParent = m_stackScreens.top();
        if (!pParent)
            return;
    }
    if (pParent->GetType() == UI_TYPE_EDIT_BOX) {
        CEditBox* pEditBox = static_cast<CEditBox*>(pParent);
        pEditBox->SetActive(false);
    }
    for (unsigned long i = 0; i < pParent->GetChildren().size(); ++i)
        DeactivateAllEditBoxes(pParent->GetChildren()[i].Get());
}
}
