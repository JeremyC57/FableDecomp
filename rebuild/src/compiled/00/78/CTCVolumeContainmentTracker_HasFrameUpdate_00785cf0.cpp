// CTCVolumeContainmentTracker::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCVolumeContainmentTracker@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00785CF0, thiscall, `ret`.
// Identity: slot 8 of CTCVolumeContainmentTracker's vtable 0x0126B66C (defined here: the slot holds this body in 1 vtable(s), all of CTCVolumeContainmentTracker or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCVolumeContainmentTracker@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCVolumeContainmentTracker.h (opt-ins as in the landed row CTCVolumeContainmentTracker_Update_00785620.cpp).
#define FABLE_RETAIL_CTCVOLUMECONTAINMENTTRACKER_UPDATE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH_BODY
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_C2DBOXI_CTORS
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH
#define FABLE_RETAIL_TC_VOLUME_CONTAINMENT_TRACKER_DEF
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_LIGHTINGCHANNEL
#define FABLE_RETAIL_CTCCARRYING_ALLCARRIED
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CSOUNDPAIR_ORIGINAL_CTORS
#define FABLE_RETAIL_CASOUNDBANK_SOUNDINDEX
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CASOUNDMANAGER_ENABLEFLAGS
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CWORLD
#include "engine/CThingBuilding.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCVolumeContainmentTracker.h"
#include "engine/CTCVolumeContainmentTrackerDef.h"
#include "engine/CWorld.h"
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_OverlappingSphere.h"
#include "engine/CThingFilter_PosInContainmentVolume.h"
#include "engine/CFilter_Bind2.h"
#include "engine/CThingFilter_HasTC.h"
#include "engine/CThingCompare_Nearest.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CTCCarrying.h"
#include "engine/CShopDef.h"
#include "engine/CThingBaseDef.h"
#include "engine/CDefinitionManager.h"
#include "engine/CASoundManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/CASoundBank.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/CSoundInit.h"
#include "engine/CSoundPair.h"
#include "engine/C2DBoxI.h"
#include "engine/ThingAlive.h"
bool CTCVolumeContainmentTracker::HasFrameUpdate() const
{
    return true;
}
