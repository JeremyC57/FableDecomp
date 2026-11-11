// CTCDoor::CTCDoor(CThing& thing) -- CTCBase(thing), not jammed, closed and still, no creature/update stamps, then the door def fetched from the parent's definition and the parent's cutscene pause behaviour set to not paused
// Leaf: ??0CTCDoor@@QAE@AAVCThing@@@Z
//
// Retail 0x00730630, 129 bytes (`ret 4`). Callees: CTCBase::CTCBase(CThing*) @ 0x00686800,
// CTCBase::GetDefinitionManager() @ 0x006869D0 (manifest "_Cons_val<...>", propagated),
// CDefinitionManager::GetDef(long, CDefPointer*) @ 0x0042AF3C, CDefClassBase::GetSubDef<CDoorDef>
// @ 0x00655D28 (manifest "GetSubDef<CAbilityDef>", a byte-twin template name), CThing::
// SetCutscenePauseBehaviour(long) @ 0x004C78A0; the def's virtual slot 1 when the temporary def pointer
// drops its last reference. Vtable 0x01266DE4 = CTCDoor (rtti-vtables.tsv).
// Layout = engine/CTCDoor.h (Ego_r.pdb, 0x24): Jammed +0xc, CurrentOpen/OldOpen/Opening/Closing
// +0x10..+0x13, LastCreatureFoundWF +0x14, UpdateID +0x18, TriggerType +0x1c (left uninitialised),
// PDef +0x20.
// Shape: one zeroed ebx for the initialisers, PDef addressed once in edi (it is the GetSubDef argument); the thing def
// handle's zero store lands after the GetDef pushes (the header's noinline GetDef body: the handle's address does not escape); the
// parent read into a named local first gives retail's eax / edx roles for the def index.
// Name fix (agentNF10 2026-11-11): moved off the local CThing / CDefPointer / CDefClassBase / CDefinitionManager / CTCBase / CTCDoor
// stand-ins onto the shared headers (engine/CTCDoor.h FABLE_RETAIL_CTCDOOR_CTOR, CTCBase.h, CThing.h FABLE_RETAIL_CTHING_CUTSCENEPAUSE,
// CDoorDef.h, CGameDefinitionManager.h). The row's own symbol is the PDB ??0CTCDoor@@QAE@AAVCThing@@@Z (was ??0CTCDoor@@QAE@PAVCThing@@@Z);
// 0x0042AF3C binds GetDef<CThingBaseDef> (was ?GetDef@CDefinitionManager@@QAE_NJPAV?$CDefPointer@VCDefClassBase@@@@@Z), 0x006869D0 the
// PDB const GetDefinitionManager, 0x00655D28 ??$GetSubDef@VCDoorDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCDoorDef@@@@@Z (FableWin.pdb;
// was ?GetSubDef@CDefClassBase@@QAE_NPAV?$CDefPointer@$$CBVCDoorDef@@@@@Z) and 0x004C78A0
// ?SetCutscenePauseBehaviour@CThing@@QAEXW4ECutsceneBehaviour@@@Z (was ..QAEXJ@Z).
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_CUTSCENEPAUSE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CDOORDEF
#define FABLE_RETAIL_CTCDOOR
#define FABLE_RETAIL_CTCDOOR_CTOR
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CDoorDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CTCDoor.h"

CTCDoor::CTCDoor(CThing& thing)
    : CTCBase(thing)
    , Jammed(0)
    , CurrentOpen(false)
    , OldOpen(false)
    , Opening(false)
    , Closing(false)
    , LastCreatureFoundWF(0)
    , UpdateID(0)
    , PDef()
{
    CDefPointer<const CThingBaseDef> def;
    CThing* parent = ParentThing;
    GetDefinitionManager().GetDef(parent->GetDefGlobalIndex(), &def);
    def->GetSubDef(&PDef);
    ParentThing->SetCutscenePauseBehaviour(CUTSCENE_BEHAVIOUR_NOT_PAUSED);
}
