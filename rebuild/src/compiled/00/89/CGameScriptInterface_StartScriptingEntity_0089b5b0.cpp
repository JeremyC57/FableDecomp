// CGameScriptInterface::StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& resource, EScriptAIPriority priority) -- refuse when an AI creature is already running a state group at a higher script priority, wait (yielding script frames) while its scripted-control component is locked, then hand the resource a new scripted-thing implementation for it; on failure give the resource a null scripted-thing handle
// Leaf: ?StartScriptingEntity@CGameScriptInterface@@UAE_NABVCScriptThing@@AAVCScriptGameResourceObjectScriptedThingBase@@W4EScriptAIPriority@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0089B5B0, 454 bytes (+ int3 padding), thiscall `ret 0Ch`. Identity: retail
//   CGameScriptInterface vtable 0x01260F0C slot 8 (engine/CGameScriptInterface.h); Ego_r.pdb / FableWin.pdb
//   ?StartScriptingEntity@CGameScriptInterface@@UAE_NABVCScriptThing@@AAVCScriptGameResourceObjectScriptedThingBase@@W4EScriptAIPriority@@@Z.
// Callees (PDB spellings): CScriptThing slots 75 IsAlive / 11 GetPThing, CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem
//   0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10, CThingAICreature::GetCurrentStateGroupScriptPriority() const 0x006D5AD0,
//   CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (GetTC, TCI_SCRIPTED_CONTROL 31), CTCScriptedControl::IsLocked()
//   (inline, +0x18), g_pScriptRunner 0x013D2838 (CASuspendableProcess::StopRunning +5), own slot 7 NewScriptFrame,
//   CScriptGameResourceObjectScriptedThing ctor 0x009039D0 (operator new 0x00BFEA1A, 0x28 bytes),
//   CScriptGameResourceObjectScriptedThingBase::SetImplementation 0x008AB960 / operator= 0x008ABD10 (out of line),
//   CScriptGameResourceObjectScriptedThing::CreateNullHandle 0x00903810 (static fastcall), CBaseObject dtor 0x0099A430,
//   operator delete 0x00BFE9BC (the null handle's inline PImp release).
// Model: shared headers (overlay opt-ins CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHING, CTCSCRIPTEDCONTROL_ISLOCKED,
//   CTHINGAICREATURE_SCRIPTPRIORITY, CGAMESCRIPTINTERFACE_GETCURRENTSCRIPTID; hdr/README.md). The prior local-model draft parked
//   on the null handle's inline PImp release reloading Info into ecx/edx for operator delete (retail eax): that reload is an
//   expression temporary on c2's eax/ecx/edx rotating pointer, and reading the script ID through the PDB's inline
//   GetCurrentScriptID() accessor (FableWin.pdb) instead of the member leaves the pointer at eax there (c2prio --rotation).
//   The creature is passed as `*static_cast<CThing*>(pthing.GetPItem())`: through Get() VC7.1 calls GetPItem before the
//   script-ID load (retail loads it first, 0x0089B6BD).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_SCRIPTPRIORITY
#define FABLE_RETAIL_CTCSCRIPTEDCONTROL_ISLOCKED
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHING
#define FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_ASSIGN
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CGAMESCRIPTINTERFACE_GETCURRENTSCRIPTID
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCScriptedControl.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CScriptGameResourceObjectScriptedThing.h"
#include "engine/CSuspendableProcessMicrothread.h"
#include "engine/CGameScriptInterface.h"
#include "engine/CScriptThing.h"

extern CSuspendableProcessMicrothread* g_pScriptRunner;                  // 0x013D2838

bool CGameScriptInterface::StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& resource, EScriptAIPriority priority)
{
    if (thing.IsAlive()) {
        CIntelligentPointer<CThing> pthing(thing.GetPThing());
        // An AI creature (CClass type 1) already running a state group above this priority stays with it.
        if (pthing->GetType() == 1 && static_cast<CThingAICreature*>(pthing.Get())->GetCurrentStateGroupScriptPriority() > priority)
            return false;
        CTCScriptedControl* control;
        pthing->GetTC(&control);
        while (control->IsLocked()) {
            if (!thing.IsAlive() || g_pScriptRunner->StopRunning)
                return false;
            NewScriptFrame();
        }
        if (pthing.Get() && !pthing->IsDead()) {
            resource.SetImplementation(new CScriptGameResourceObjectScriptedThing(*this, *static_cast<CThing*>(pthing.GetPItem()), GetCurrentScriptID(), priority));
            return true;
        }
    }
    resource = CScriptGameResourceObjectScriptedThing::CreateNullHandle(*this);
    return false;
}