// NUISystem::CSlider::CSlider(long id) -- front-end slider: CSprite base, this class's three vtables, the action vector and the action-list / arrow / accept-button pointers zeroed, then GetDataFromDef()
// Leaf: ??0CSlider@NUISystem@@QAE@J@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0054C050, length 102 bytes (`ret 4`).
// Callees: CSprite::CSprite(long) @ 0x0041B800, CSlider::GetDataFromDef @ 0x0054B4B0 (Ego_r.pdb + FableWin.pdb ?GetDataFromDef@CSlider@NUISystem@@QAEXXZ,
//   the catalogue name of that row). Vtables (rtti-vtables.tsv NUISystem::CSlider): 0x01248D04 at +0, 0x01248CE0 at +4 (CObserver), 0x01248CD8 at
//   +0x18 (CActionParam).
// Identity: Ego_r.pdb + FableWin.pdb ??0CSlider@NUISystem@@QAE@J@Z (engine/NUISystem_CSlider.h declares it for 0x0054C050).
// Shape: one zero register for the init-list zero members (vector ctor first); PreviousValue (+0x1E0) is left to GetDataFromDef().
// NF79 (round 7, q10962 / q10963 follow-up): ported onto engine/NUISystem_CSlider.h; was a local CSprite / interface / CSlider model calling
//   0x0054B4B0 as the made-up Initialize, with made-up vtable base names (IPrimary_0054c050 / ISecondaryB_0054c050).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#include "engine/CCountedPointer.h"
#include "engine/NUISystem_CSlider.h"

namespace NUISystem {
CSlider::CSlider(long id)
    : CSprite(id), PActionsOnBack(0), PActionsOnSelect(0), PDefAction(EActionType(0)), PLeftArrow(0), PRightArrow(0), PAcceptButton(0)
{
    GetDataFromDef();
}
}
