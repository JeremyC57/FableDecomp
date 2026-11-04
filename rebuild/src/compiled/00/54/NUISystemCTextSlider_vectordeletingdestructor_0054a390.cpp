// NUISystem::CTextSlider::`vector deleting destructor'(unsigned int) adjustor thunk -- this-4 (the CObserver subobject at +4; `sub ecx,4 ; jmp ??_G`)
// Leaf: ??_ECTextSlider@NUISystem@@W3AEPAXI@Z
// Flags: none
//
// Retail 0x0054A390, 8 bytes: the VC7.1 adjustor thunk in slot 0 of CTextSlider's CObserver vtable 0x01248A68 (+4); it
// moves `this` back to the full object and jumps to the deleting destructor ??_G 0x0054A440. VC7.1 names the slot-0 entry
// of a class with no array delete `??_E` and emits the thunk in the TU that defines the virtual destructor (precedent:
// CScrollingViewport 0x0053FC10 / 0x0053FC20, CWorldMap 0x0051CDF0), so this source is the destructor's TU:
// ~CTextSlider (0x00549A00, landed) defined here makes the compiler emit ??_G and both thunks.
// Callees: ??_GCTextSlider@NUISystem@@UAEPAXI@Z 0x0054A440.
// Header: engine/NUISystem_CTextSlider.h (new).
#define FABLE_RETAIL_CACTIONPARAMSLONG
#include "engine/NUISystem_CTextSlider.h"
#include "engine/NUISystem_CClickable.h"
#include "engine/NUISystem_CActionParamSLONG.h"
#include "engine/NUISystem_CManager.h"
namespace NUISystem {
CTextSlider::~CTextSlider()
{
    if (StartSelectedChild != SelectedChild) {
        for (unsigned int i = 0; i < Actions.size(); ++i) {
            static_cast<CActionParamSLONG*>(*Actions[i]->UIActionParams.begin())->Value = StartSelectedChild;
            CManager::GetInstance()->Action(Actions[i]);
        }
        Move = true;
    }
    delete PActionsOnBack;
    delete PActionsOnSelect;
}
}