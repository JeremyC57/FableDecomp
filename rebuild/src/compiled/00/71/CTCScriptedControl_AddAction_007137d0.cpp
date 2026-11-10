// CTCScriptedControl::AddAction(CTCScriptedControl::CActionBase* action) -- attach the action to this control (SetScript, then its
// OnAdd, virtual slot 3), wrap it in a CCountedPointer and append it to PActionList.
// Leaf: AddAction
// Retail: Fable.exe 0x007137D0, 126 bytes (`ret 4`).
// Name: Ego_r.pdb + FableWin.pdb ?AddAction@CTCScriptedControl@@QAEXPAVCActionBase@1@@Z (the nested action base; engine/CTCScriptedControl.h
// ADD_ACTION; rows/00903f30/NAMING_EVIDENCE.md).
// Callees: CActionBase::OnAdd (virtual slot 3, +0x0C), operator new 0x00BFEA1A (the 12-byte CCPPointerInfo), malloc 0x00BFEA0E
// (STLport node allocator, 16-byte list node), operator delete 0x00BFE9BC (the count block); the count block's DeleteFunc is
// CCountedPointer<CTCScriptedControl::CActionBase>::DeleteData 0x007180C0.
// Layout = Ego_r.pdb CTCScriptedControl (0x24 here): PActionList (STLport list<CCountedPointer<CActionBase> >) +0x14 (overlay opt-in
// FABLE_RETAIL_CTCSCRIPTEDCONTROL_ACTIONLIST); CActionBase: PTCScript +0x04.
// Shape: a temporary CCountedPointer built from the raw pointer, STLport push_back = insert(end()) with the placement copy's null
// test; the temporary's release keeps no zero stores (register-only local).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTCSCRIPTEDCONTROL_ACTIONBASE
#define FABLE_RETAIL_CTCSCRIPTEDCONTROL_ADD_ACTION
#define FABLE_RETAIL_CTCSCRIPTEDCONTROL_ACTIONLIST
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCScriptedControl.h"

void CTCScriptedControl::AddAction(CActionBase* action)
{
    action->SetScript(this);
    action->OnAdd();
    PActionList.push_back(CCountedPointer<CActionBase>(action));
}
