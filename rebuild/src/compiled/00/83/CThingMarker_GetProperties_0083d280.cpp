// CThingMarker::GetProperties() const -- the marker def's Properties word, read through the by-value PeekDefinition
// Leaf: ?GetProperties@CThingMarker@@UBEKXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0083D280, length 94 bytes (CThingMarker vtable 0x01245524 slot 48).
// Identity: Ego_r.pdb ?GetProperties@CThingMarker@@UBEKXZ (public const virtual).
// Callees: CThing::PeekDefinitionManager 0x004C7990, CDefinitionManager::GetDef<CThingMarkerDef> 0x0083D630, CThingMarkerDef virtual
//   slot 1 (Delete at a zero count).
// Shape: the inline PeekDefinition (Ego_r ?PeekDefinition@CThingMarker@@IBE?AV?$CDefPointer@$$CBVCThingMarkerDef@@@@XZ, by value) fills
//   a local handle and returns it by copy (add-ref and release share one null test, both count stores kept), then `->Properties` is
//   read and the returned temporary released without a null test.
// Layout: CThing::DefGlobalIndex +0x8C; CThingMarkerDef::Properties +0x84 (Ego_r +0x88).
// Name fix (agentNF9 2026-11-11): moved off the local CThing / CThingMarker / CThingMarkerDef / CDefinitionManager models (a same-TU
//   stand-in GetDef_0083d630 body) onto the shared headers; 0x004C7990 binds ?PeekDefinitionManager@CThing@@QBEABVCGameDefinitionManager@@XZ,
//   0x0083D630 ??$GetDef@VCThingMarkerDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCThingMarkerDef@@@@@Z; the row's own symbol
//   is the const virtual the PDB spells (was ?GetProperties@CThingMarker@@UAEKXZ).
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

unsigned long CThingMarker::GetProperties() const
{
    return PeekDefinition()->Properties;
}
