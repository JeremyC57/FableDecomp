// NUISystem::CSlider::`vector deleting destructor'(unsigned int) adjustor thunk -- this-4 (the CObserver subobject at +4; `sub ecx,4 ; jmp ??_G`)
// Leaf: ??_ECSlider@NUISystem@@W3AEPAXI@Z
// Flags: none
//
// Retail 0x0054C140, 8 bytes: the VC7.1 adjustor thunk in slot 0 of CSlider's CObserver vtable 0x01248CE0 (+4); it moves
// `this` back to the full object and jumps to the deleting destructor ??_G 0x0054C1B0 (slot 0 of the primary vtable 0x01248D04).
// VC7.1 names the slot-0 entry of a class with no array delete `??_E` and emits the thunks in the TU that defines the virtual
// destructor (precedent: CDraggableInto 0x00559020 / 0x00559030, CScrollingViewport 0x0053FC10 / 0x0053FC20), so this source
// is the destructor's TU: ~CSlider (0x0054B360, landed) defined here makes the compiler emit ??_G and both thunks.
// Callees: ??_GCSlider@NUISystem@@UAEPAXI@Z 0x0054C1B0.
// Header: engine/NUISystem_CSlider.h (new, chunk 752).
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/NUISystem_CSlider.h"
#include "engine/NUISystem_CClickable.h"
#include "engine/NUISystem_CActionParamFloat.h"
#include "engine/NUISystem_CManager.h"
namespace NUISystem {
CSlider::~CSlider()
{
    if (!(StartValue == Value)) {
        for (unsigned int i = 0; i < Actions.size(); ++i) {
            static_cast<CActionParamFloat*>(Actions[i]->UIActionParams.front())->Value = StartValue.X;
            CManager::GetInstance()->Action(Actions[i]);
        }
        Move = true;
    }
    delete PActionsOnBack;
    delete PActionsOnSelect;
}
}