// CThingMarker::GetCurrentGraphic() const -- the marker's current graphic: the base def's appearance sub-def graphic when it names a bank, else the marker def's own graphic; returned by value (hidden result pointer)
// Leaf: ?GetCurrentGraphic@CThingMarker@@IBE?AVCEngineGraphic@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0083D190, length 231 bytes (`ret 4`; 240 with padding).
// Identity: Ego_r.pdb ?GetCurrentGraphic@CThingMarker@@IBE?AVCEngineGraphic@@XZ (protected const, by value); the only caller is 0x0083D31E
//   inside the CThingMarker virtual 0x0083D2E0. The catalog label CThingVillage::GetCurrentGraphic is propagated from the twin 0x0083B520.
// Callees: CDefClassBase::GetSubDef<CAppearanceDef> 0x0042B0A2 (on the base def), CThing::PeekDefinitionManager 0x004C7990,
//   CDefinitionManager::GetDef<CThingMarkerDef> 0x0083D630, the defs' Delete (slot 1).
// Layout: CThing PBaseDef +0x70, DefGlobalIndex +0x8C; CAppearanceDef::Graphic +0x25 (packed); CThingMarkerDef::Graphic +0x70;
//   CEngineGraphic is 14 packed bytes; BankIndex is unsigned (`> 0` -> `jbe`).
// Name fix (agentNF9 2026-11-11): moved off the local models (descriptive CVillageStateDef / CThingVillageDef stand-ins, same-TU stand-in
//   GetSubDef / GetDef bodies) onto the shared headers; 0x0042B0A2 binds
//   ??$GetSubDef@VCAppearanceDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCAppearanceDef@@@@@Z (was
//   ?GetSubDef@CThingBaseDef@@QBE_NPAV?$CDefPointer@UCVillageStateDef@@@@@Z), 0x0083D630 the PDB GetDef<CThingMarkerDef>, 0x004C7990
//   ?PeekDefinitionManager@CThing@@QBEABVCGameDefinitionManager@@XZ; the row's own symbol is the protected const PDB spelling.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CTHINGMARKER_DEFINITION
#define FABLE_RETAIL_CAPPEARANCEDEF_COMPLETE
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CAppearanceDef.h"
#include "engine/CThingMarker.h"

CEngineGraphic CThingMarker::GetCurrentGraphic() const
{
    CDefPointer<const CAppearanceDef> appearance;
    if (PBaseDef->GetSubDef(&appearance) && appearance->Graphic.BankIndex > 0)
        return appearance->Graphic;
    return PeekDefinition()->Graphic;
}
