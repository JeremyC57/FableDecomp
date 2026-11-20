// NUISystem::CSlider::`scalar deleting destructor'(unsigned int flags) -- run ~CSlider, free the control when bit 0 is set
// Leaf: ??_GCSlider@NUISystem@@UAEPAXI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x0054C1B0, 30 bytes, thiscall, `ret 4`. Compiler-generated.
// Identity (two sources): slot 0 of CSlider's primary vtable 0x01248D04 (RTTI .?AVCSlider@NUISystem@@); it calls ~CSlider 0x0054B360
//   (Ego_r.pdb / FableWin.pdb ??1CSlider@NUISystem@@UAE@XZ) and operator delete 0x00BFE9BC; the matched adjustor thunks 0x0054C140 (+4) /
//   0x0054C150 (+0x18) jump to it. VC7.1 emits the function as ??_G and the vtable / thunk entry ??_ECSlider@NUISystem@@UAEPAXI@Z
//   (Ego_r.pdb's slot-0 name, egor_pdb_names 0x005C6060) as a weak alias of it (objdump -t: scl 105; no array new in the TU).
// Was: the free stand-in CSlider_scalar_deleting_destructor calling CSlider_dtor (struct CSlider model, rebuild/src/compiled/00/54/
//   NUISystemCSlider_scalardeletingdestructor_0054c1b0.cpp).
// TU: the destructor's TU, as the matched thunks 0x0054C140 / 0x0054C150 (~CSlider 0x0054B360 defined here makes VC7.1 emit ??_G and both
//   thunks; it is its own row, not graded here, and is not expanded into the ??_G: the call stays, as in retail). The flags are the
//   thunk rows' (none), as at 0x0054C140 / 0x0054C150.
// Header: engine/NUISystem_CSlider.h.
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
