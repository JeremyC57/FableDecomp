// CThingCreatureBase::GetCurrentMeshRadius() const -- the creature def's radius spread interpolated by its hero-stat radius weight for this thing's hero experience; 1 when the thing has no experience component or no creature def
// Leaf: ?GetCurrentMeshRadius@CThingCreatureBase@@QBEMXZ
// Flags: none
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_MESHSIZE
#define FABLE_RETAIL_CCREATUREDEF_TRANSFER
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingCreatureDef.h"
#include "engine/CCreatureDef.h"
#include "engine/CHeroAbilityStatWeight.h"
#include "engine/CDefClassBase_GetSubDef.h"
