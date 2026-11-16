// CTCCombat::IsInMelee() const -- forwards to the parent creature (+0x24): `mov ecx,[ecx+24h] ; jmp 0x00661E80` (CThingCreatureBase::IsInMelee). Ego_r.pdb 0x0074B330 ?IsInMelee@CTCCombat@@QBE_NXZ; Ego_r order 0x0074B310..0x0074B340 = retail 0x00733170..0x007331A0; the header ISINMELEE opt-in names 0x00733190.
// Leaf: ?IsInMelee@CTCCombat@@QBE_NXZ
// NF37 (2026-11-16): moved off its stand-in onto the shared engine/CTCCombat.h; include chain = the preamble of the landed 00/74/CTCPerceiveThing_UpdateLineOfSight_00745120.cpp.
#define FABLE_RETAIL_HEARING
#define FABLE_RETAIL_CTCPERCEIVETHING_SMELL
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCCOMBAT
#define FABLE_RETAIL_CTCCOMBAT_ISINMELEE
#define FABLE_RETAIL_CCOMBATABILITY
#define FABLE_RETAIL_CCOMBATABILITYDATA
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_LINEOFSIGHT
#define FABLE_RETAIL_STEALTH_DRAW
#define FABLE_RETAIL_CTCSTEALTH_STATTYPE
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_PERCEIVERS
#define FABLE_RETAIL_CTHINGFILTER_ISINFOV_CTOR
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_FILTER_BIND3_FUNCTION
#define FABLE_RETAIL_C2DBOXI_CTORS
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_RESERVE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREABODY
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTCMAPWHO_STATTYPE
#include "engine/CTCPerceiveThing.h"
#include "engine/CPerceivedThingDef.h"
#include "engine/CTCStealth.h"
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCCombat.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CPhysicsMeshObject.h"
#include "engine/CMeshIntersectionData.h"
#include "engine/CThingFilter_IsInFOV.h"
#include "engine/CThingFilter_IsNot.h"
#include "engine/CThingFilter_HasPhysicsMesh.h"
#include "engine/CThingFilter_RadiusIntersectsInfiniteLine.h"
#include "engine/CFilter_Bind3.h"
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/CWorldMap.h"
#include "engine/CWorld.h"
#include "engine/CPlayerGui.h"
#include "engine/C2DBoxI.h"

bool CTCCombat::IsInMelee() const
{
    return ParentCreature->IsInMelee();
}
