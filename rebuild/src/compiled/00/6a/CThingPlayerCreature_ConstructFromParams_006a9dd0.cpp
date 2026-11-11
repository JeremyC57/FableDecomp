// CThingPlayerCreature::ConstructFromParams(long defIndex, const C3DVector& position, long playerNumber, const CThingPlayerCreatureInit& init) -- report progress, construct the creature base, reset health from the creature def, look up the appearance sub-def and add the physics-controlled component
// Leaf: ?ConstructFromParams@CThingPlayerCreature@@IAE_NJABVC3DVector@@JABVCThingPlayerCreatureInit@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A9DD0, 285 bytes (+ int3 padding), `ret 10h`. Ego_r.pdb / FableWin.pdb
//   ?ConstructFromParams@CThingPlayerCreature@@IAE_NJABVC3DVector@@JABVCThingPlayerCreatureInit@@@Z (protected).
// Callees (PDB spellings): CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, NProgressDisplay::DisplayProgress
//   0x009E9F40 (__fastcall), CThingCreatureBase::ConstructFromParams 0x00662880, CDefClassBase::GetSubDef<CAppearanceDef> 0x0042B0A2
//   (on the base def; the handle is only released), CThing::AddTC 0x004C9D60, the def's virtual release.
// Literals: "CThingPlayerCreature::ConstructFromParams 1" 0x0125D188, "... 2" 0x0125D15C, "" 0x0122D70E, "CTCPhysicsControlled" 0x01239F4C.
// Layout: CThingPlayerCreatureInit (Ego_r.pdb, 12 bytes); CThingPhysical MaxHealth +0xB0 / Health +0xB4; CThingCreatureBase PDef +0xE0
//   (CThingBaseDef Health +0x50); CThing PBaseDef +0x70.
// Shape: every string is an implicit CCharString temporary; `MaxHealth = Health = PDef->Health` (Health stored first).
// Name fix (agentNF11 2026-11-11): moved off the local CThing / CThingPhysical / CThingCreatureBase / CThingPlayerCreature / CDefPointer /
// CCharString stand-ins (and a same-TU stand-in GetSubDef body) onto the shared headers (new engine/NProgressDisplay.h,
// engine/CThingPlayerCreatureInit.h). 0x0042B0A2 binds GetSubDef<CAppearanceDef> (was ?GetSubDef@..V?$CDefPointer@VCPlayerCreatureSubDef@@@@@Z,
// an invented sub-def type), 0x00662880 the PDB ConstructFromParams (references), the literals are literals (were g_str* globals); the
// row's own symbol takes the PDB reference parameters.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_CONSTRUCT
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE_CONSTRUCT
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CRESOURCELIST
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CAnimationEntry.h"
#include "engine/CAppearanceDef.h"
#include "engine/CThingCreatureDef.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CThingPlayerCreatureInit.h"
#include "engine/CDefinitionManager.h"
#include "engine/NProgressDisplay.h"

bool CThingPlayerCreature::ConstructFromParams(long defIndex, const C3DVector& position, long playerNumber, const CThingPlayerCreatureInit& init)
{
    NProgressDisplay::DisplayProgress("CThingPlayerCreature::ConstructFromParams 1", -1.0f, false, false);
    if (CThingCreatureBase::ConstructFromParams(defIndex, position, playerNumber, init.OriginalThing, init.PreviousThing, "", init.PlaceAtEndOfList)) {
        NProgressDisplay::DisplayProgress("CThingPlayerCreature::ConstructFromParams 2", -1.0f, false, false);
        MaxHealth = Health = PDef->Health;
        CDefPointer<const CAppearanceDef> appearance;
        PBaseDef->GetSubDef(&appearance);
        AddTC("CTCPhysicsControlled", true, 0);
        return true;
    }
    return false;
}
