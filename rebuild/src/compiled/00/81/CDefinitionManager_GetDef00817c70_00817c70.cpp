// Leaf: ??$GetDef@VCHitLocationDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCHitLocationDef@@@@@Z
// Flags: /Gr
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_HITLOCATIONS_BOUND
#define FABLE_RETAIL_HITLOCATIONS_ACTIVATE_PASS30
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD
#define FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL_DECL
#include "rebuild_abi.h"
#include "engine/CThing.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CThingBaseDef.h"
#include "engine/CHitLocationsDef.h"
#include "engine/CArmourDef.h"
#include "engine/CTCHitLocations.h"

template bool CDefinitionManager::GetDef<CHitLocationDef>(long, CDefPointer<const CHitLocationDef>*) const;
