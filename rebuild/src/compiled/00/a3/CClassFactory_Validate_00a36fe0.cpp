// CClassFactory::Validate() -- validate every allocated class: call CBaseObject::Validate (virtual slot 1) on each entry of ClassList
// Leaf: ?Validate@CClassFactory@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A36FE0, 34 bytes: `this` in edi, the node in esi, the list end (+0x18) re-read for the loop test,
//   the loop head aligned, `call [eax+4]` (CBaseObject::Validate, CClass vtable 0x0129CF60 slot 1), `ret`.
// Naming (NF446, 2026-11-25): self respelt from the made-up ?CallAll@CListOwner_00a36fe0@@QAEXXZ (catalog CListOwner_00a36fe0::CallAll; the
//   propagated chunk label CNavigatorManager::InterruptAllNavigators belongs to 0x00A14F40). Evidence: both PDB name sets (Ego_r 0x00B76920,
//   FableWin 0x03193FF0 ?Validate@CClassFactory@@QAEXXZ); FableWin's /Od CGameScriptInterface::Validate calls it on World->GetThingManager()
//   (CThingManager : CClassFactory) and retail 0x006E72A0 tail-jumps to 0x00A36FE0 with that object in ecx (the only retail reference, exe
//   scan); the body walks ClassList (+0x18, std::list<CClass*>, engine/CClassFactory.h) calling each class's CBaseObject::Validate.
// Header: engine/CClassFactory.h FABLE_RETAIL_CCLASSFACTORY_CLASSLIST (typed ClassList) + the add-only NF435 opt-in
//   FABLE_RETAIL_CCLASSFACTORY_VALIDATE (declaration), engine/CClass.h (FABLE_RETAIL_CCLASS) for the CBaseObject base.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CCLASSFACTORY_CLASSLIST
#define FABLE_RETAIL_CCLASSFACTORY_VALIDATE
#include "engine/CClassFactory.h"
#include "engine/CClass.h"

void CClassFactory::Validate()
{
    for (std::list<CClass*>::iterator it = ClassList.begin(); it != ClassList.end(); ++it)
        (*it)->Validate();
}
