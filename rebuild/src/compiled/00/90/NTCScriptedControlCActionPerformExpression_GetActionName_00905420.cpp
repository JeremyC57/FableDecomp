// NTCScriptedControl::CActionPerformExpression::GetActionName() const -- the action's name, "CActionPerformExpression"
// Leaf: ?GetActionName@CActionPerformExpression@NTCScriptedControl@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00905420, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 4 of NTCScriptedControl::CActionPerformExpression's vtable 0x0128D8EC (the slot holds this body in 1 vtable(s), owner NTCScriptedControl::CActionPerformExpression); the Xbox 360 Test PDB virtual order (third source) aligned on 2 TLC-named slots of that vtable puts GetActionName(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetActionName@CActionPerformExpression@NTCScriptedControl@@UBE?AVCCharString@@XZ; `ret` matches the signature.
// Was: NTCScriptedControl::CActionPerformExpression::vfunc_4 (rebuild/src/compiled/00/90/CActionDoCreatureAction_GetActionName_00905420.cpp).
// No shared header for NTCScriptedControl::CActionPerformExpression: minimal local definition on its RTTI base
// CTCScriptedControl::CActionBase (engine/CTCScriptedControl.h).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCSCRIPTEDCONTROL_ACTIONBASE
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCScriptedControl.h"
namespace NTCScriptedControl {
class CActionPerformExpression : public CTCScriptedControl::CActionBase {
public:
    virtual CCharString GetActionName() const;               // slot 4
};
CCharString CActionPerformExpression::GetActionName() const
{
    return CCharString("CActionPerformExpression");
}
}
