// CTCScriptedControl::CActionBase::OnAdd() -- a scripted action does nothing when it is added
// Leaf: ?OnAdd@CActionBase@CTCScriptedControl@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00711FE0, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 3 of NTCScriptedControl::CActionPerformExpression's vtable 0x0128D8EC (the slot holds this body in 14 vtable(s), owner CTCScriptedControl::CActionBase); the Xbox 360 Test PDB virtual order (third source) aligned on 2 TLC-named slots of that vtable puts OnAdd(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnAdd@CActionBase@CTCScriptedControl@@UAEXXZ; `ret` matches the signature.
// Was: CTCScriptedControl::CActionBase::vfunc_3 (rebuild/src/compiled/00/71/gapscan_sub00711fe0_00711fe0.cpp).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCSCRIPTEDCONTROL_ACTIONBASE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCScriptedControl.h"
void CTCScriptedControl::CActionBase::OnAdd()
{
}
