// NUISystem::CFrontEndButton::~CFrontEndButton() -- an empty user body: the vptrs, then the members in reverse order (the UIActionParams
//   list out of line, the two CCountedPointer<CAction> vectors inline: range destroy, free) and the CClickable base destructor
// Leaf: ??1CFrontEndButton@NUISystem@@UAE@XZ
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x0054DED0, 127 bytes, `ret`. Callees: the list<CActionParam*> destructor 0x0042AB1C, __destroy_aux
//   0x0054A1C0 (`__fastcall` first / last, the __false_type tag by reference), free 0x00BFEA14 (x2), ~CClickable 0x0055B760.
// Identity (two sources): Ego_r.pdb / FableWin.pdb ??1CFrontEndButton@NUISystem@@UAE@XZ; its only caller is the ??_G 0x0054E1C0, slot 0 of
//   CFrontEndButton's vtable 0x01249554 (it stores 0x01249554 / 0x01249530 / 0x01249528); the members are the Ego_r.pdb ones
//   (engine/NUISystem_CFrontEndButton.h FABLE_RETAIL_CFRONTENDBUTTON_MEMBERS: Actions +0x198, ActionsOnDelete +0x1A4, UIActionParams +0x1B0).
// Was: the same destructor on local SControlBase0/1/2 + CFrontEndControl_0055b760 / CActionVector / CActionParamList models
//   (rebuild/src/compiled/00/54/NUISystemCFrontEndButton_CFrontEndButton_0054ded0.cpp), which bound ~CClickable 0x0055B760 under the
//   stand-in ??1CFrontEndControl_0055b760@@QAE@XZ.
// Now: on the shared headers, as the matched thunk row 0x0054E1B0 defines it. /Gr: the CFrontEndButton TU calls STLport's __destroy_aux as
//   `__fastcall` (the landed 0x0054A1C0 row is a /Gr TU; the old row spelled the free functions FABLE_FASTCALL by hand).
// Header: engine/NUISystem_CFrontEndButton.h (FABLE_RETAIL_CFRONTENDBUTTON_MEMBERS), stlport/fable_containers.h (FABLE_RETAIL_STLP_LIST).
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CFRONTENDBUTTON_MEMBERS
#include "engine/NUISystem_CFrontEndButton.h"
namespace NUISystem {
CFrontEndButton::~CFrontEndButton()
{
}
}
