// NUISystem::CFrontEndButton::`scalar deleting destructor'(unsigned int flags) -- run ~CFrontEndButton, free the control when bit 0 is set
// Leaf: ??_GCFrontEndButton@NUISystem@@UAEPAXI@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0054E1C0, 30 bytes, thiscall, `ret 4`. Compiler-generated.
// Identity (two sources): slot 0 of CFrontEndButton's primary vtable 0x01249554 (RTTI .?AVCFrontEndButton@NUISystem@@); it calls
//   ~CFrontEndButton 0x0054DED0 (Ego_r.pdb / FableWin.pdb ??1CFrontEndButton@NUISystem@@UAE@XZ) and operator delete 0x00BFE9BC; the matched
//   adjustor thunks 0x0054E1A0 / 0x0054E1B0 jump to it. VC7.1 emits the function as ??_G and the vtable / thunk entry
//   ??_ECFrontEndButton@NUISystem@@UAEPAXI@Z as a weak alias of it (objdump -t: scl 105; no array new in the TU).
// Was: the stand-in CFrontEndButton::ScalarDeletingDestructor calling CFrontEndButton::Dtor (?Dtor@CFrontEndButton@@QAEXXZ, local struct
//   model, rebuild/src/compiled/00/54/NUISystemCFrontEndButton_scalardeletingdestructor_0054e1c0.cpp).
// TU: the destructor's TU, the text of row 0x0054DED0 (~CFrontEndButton over the FABLE_RETAIL_CFRONTENDBUTTON_MEMBERS members, /Gr as
//   that row; it is its own row, not graded here, and stays a call in the ??_G, as in retail). On the header's opaque default branch (the
//   0x0054E1B0 thunk TU) the empty destructor is expanded into the ??_G (DIFFER 50v30). Only the ??_G is this row.
// Header: engine/NUISystem_CFrontEndButton.h (FABLE_RETAIL_CFRONTENDBUTTON_MEMBERS), stlport/fable_containers.h (FABLE_RETAIL_STLP_LIST).
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CFRONTENDBUTTON_MEMBERS
#include "engine/NUISystem_CFrontEndButton.h"
namespace NUISystem {
CFrontEndButton::~CFrontEndButton()
{
}
}
