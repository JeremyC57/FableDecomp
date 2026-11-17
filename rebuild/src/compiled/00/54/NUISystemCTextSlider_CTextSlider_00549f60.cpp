// NUISystem::CTextSlider::CTextSlider(long id) -- construct a text slider: changing-state base, empty action vector, null pointers, then GetDataFromDef()
// Leaf: ??0CTextSlider@NUISystem@@QAE@J@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00549F60, 114 bytes. thiscall, `ret 4`.
// Callees: NUISystem::CChangingStateComponent::CChangingStateComponent(long) 0x0052CC50; NUISystem::CTextSlider::GetDataFromDef() 0x00549B20
//          (Ego_r.pdb 0x005C3860 + FableWin.pdb ?GetDataFromDef@CTextSlider@NUISystem@@QAEXXZ; NF76 re-authored that row under it).
//          Vtables 0x01248A8C / 0x01248A68 / 0x01248A60 (.?AVCTextSlider@NUISystem@@) at +0 / +4 / +0x18.
// Shape: one `xor eax,eax` shared by the init-list zeroes in declaration order: Actions (3 words), PActionsOnBack,
//        PActionsOnSelect, InputDelay, PDefAction, PLeftArrow, PRightArrow, PAcceptButton, MaxChild (the selection words,
//        Move, LastInputTime and OriginalSelectedChild are left to GetDataFromDef()).
// Layout: engine/NUISystem_CTextSlider.h (retail = PDB - 0x30 up to Actions, - 0x34 after the 12-byte STLport vector).
// NF79 (round 7, q10961): ported onto the shared header (NF76's GetDataFromDef 0x00549B20 macro block); the local CTextSlider /
//   CChangingStateComponent / interface / CRetailVector models are gone. 0x00549B20 now binds GetDataFromDef (was the made-up
//   ?Initialize@CTextSlider@NUISystem@@QAEXXZ) and the three vtables their header spellings.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_VECTOR
#include "engine/CCountedPointer.h"
#include "engine/NUISystem_CTextSlider.h"

namespace NUISystem {
CTextSlider::CTextSlider(long id)
    : CChangingStateComponent(id), PActionsOnBack(0), PActionsOnSelect(0), InputDelay(0), PDefAction(EActionType(0)), PLeftArrow(0),
      PRightArrow(0), PAcceptButton(0), MaxChild(0)
{
    GetDataFromDef();
}
}
