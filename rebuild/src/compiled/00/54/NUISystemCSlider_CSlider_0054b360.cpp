// NUISystem::CSlider::~CSlider() -- if the value moved, push the starting value back into every action's target and re-run the actions;
//   delete the back / select action lists; then the member and CSprite base destructors
// Leaf: ??1CSlider@NUISystem@@UAE@XZ
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0054B360, 328 bytes, `ret`. Callees: CManager::GetInstance 0x0041E5F2 and its slot 14 (+0x38),
//   list<CCountedPointer<CAction> >::clear 0x005356A0, free 0x00BFEA14, operator delete 0x00BFE9BC, __destroy_aux 0x0054A1C0 (`__fastcall`:
//   first ecx, last edx, the __false_type tag on the stack), CSprite::~CSprite 0x0041AB40; the shared epsilon DELTA_NOTIONAL_ZERO 0x0129BA3C.
// Identity (two sources): Ego_r.pdb / FableWin.pdb ??1CSlider@NUISystem@@UAE@XZ (virtual: CSlider's vtable 0x01248D04 slot 0 is the ??_G
//   0x0054C1B0, which calls this body); the body stores the three CSlider vptrs 0x01248D04 / 0x01248CE0 / 0x01248CD8.
// Was: the same body on a local pack(1) CSlider / CSprite / CRetailVector / CRetailList / CManager model whose non-virtual declaration
//   gave ??1CSlider@NUISystem@@QAE@XZ (rebuild/src/compiled/00/54/NUISystemCSlider_CSlider_0054b360.cpp).
// Now: the destructor as the matched thunk row 0x0054C140 defines it on the shared engine/NUISystem_CSlider.h (CSprite base,
//   std::vector / std::list members, CActionParamFloat target). /Gr: retail calls the STLport __destroy_aux as `__fastcall` (the landed
//   0x0054A1C0 row is a /Gr TU); without /Gr VC7.1 pushes first / last (DIFFER 333v328).
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
