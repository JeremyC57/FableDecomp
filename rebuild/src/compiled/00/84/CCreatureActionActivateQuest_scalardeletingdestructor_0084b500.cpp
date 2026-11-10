// CCreatureAction_ActivateQuest::`scalar deleting destructor'(unsigned int) -- the compiler-generated deleting destructor: the implicit ~CCreatureAction_ActivateQuest expanded (~CBaseIntelligentPointer on PObjectToUse +0xAD, ~CCharString on ScriptName +0xA8, then the out-of-line ~CCreatureActionBase 0x00693EF0), then operator delete when bit 0 of the flags is set
// Leaf: ??_GCCreatureAction_ActivateQuest@@UAEPAXI@Z
// Retail: Fable.exe @ 0x0084B500. Identity: slot 0 of vtable 0x012752C4, RTTI .?AVCCreatureAction_ActivateQuest@@ (the catalog name agrees).
// Callees: CBaseIntelligentPointer::~CBaseIntelligentPointer 0x00A01C10 (Ego_r.pdb / FableWin.pdb ??1CBaseIntelligentPointer@@UAE@XZ),
//   CCharString::~CCharString 0x0099EAE0, CCreatureActionBase::~CCreatureActionBase 0x00693EF0, operator delete 0x00BFE9BC.
// Model: the shared engine/CCreatureAction_ActivateQuest.h (hdr overlay opt-in FABLE_RETAIL_CCREATUREACTION_ACTIVATEQUEST, the retail
//   class) over engine/CCreatureActionBase.h and engine/CIntelligentPointer.h (retail branches).
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_ACTIVATEQUEST
#include "fable_string.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCreatureAction_ActivateQuest.h"

// Emission harness (not a retail function): an inline copy construction emits the vftable and its deleting destructor.
CCreatureActionBase* EmitActivateQuestDeletingDestructor(const CCreatureAction_ActivateQuest& action)
{
    return new CCreatureAction_ActivateQuest(action);
}
