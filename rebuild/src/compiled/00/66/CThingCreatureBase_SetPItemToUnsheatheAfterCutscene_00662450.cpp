// CThingCreatureBase::SetPItemToUnsheatheAfterCutscene(CThing* item) -- remember the item to unsheathe once the cutscene ends (+0x150)
// Leaf: ?SetPItemToUnsheatheAfterCutscene@CThingCreatureBase@@QAEXPAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00662450, 74 bytes, thiscall `ret 4`: a CIntelligentPointer<CThing> temporary from the argument
//   (CBaseIntelligentPointer() 0x00A01B10 + SetPItem 0x00A01B90, vtable 0x01238C6C), assigned to PItemToUnsheatheAfterCutscene +0x150
//   (GetPItem 0x00A01B50 + SetPItem), then destroyed (~CBaseIntelligentPointer 0x00A01C10).
// Name (NF70, q10817): the spelling engine/CThingCreatureBase.h gives it (FABLE_RETAIL_CTHINGCREATUREBASE_SETPITEMTOUNSHEATHE, NF66;
//   DESCRIPTIVE: no PDB / FSE / Ego_d name; the member it sets is Ego_r's PItemToUnsheatheAfterCutscene) and its landed caller 0x00694430
//   binds; was the stand-in CClass_00662450::AssignTarget_00662450(CBaseObject*) on a local model.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN   // the retail inline operator= (GetPItem 0x00A01B50 then SetPItem), as chunk 754
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_SETPITEMTOUNSHEATHE
#define FABLE_RETAIL_C3DVECTOR
#include "fable_compiler.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
void CThingCreatureBase::SetPItemToUnsheatheAfterCutscene(CThing* item)
{
    PItemToUnsheatheAfterCutscene = CIntelligentPointer<CThing>(item);
}
