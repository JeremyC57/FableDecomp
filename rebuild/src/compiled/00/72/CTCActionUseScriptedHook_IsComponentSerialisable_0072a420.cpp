// CTCActionUseScriptedHook::IsComponentSerialisable() const -- returns true
// Leaf: ?IsComponentSerialisable@CTCActionUseScriptedHook@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0072A420, thiscall, `ret`.
// Identity: slot 23 of CTCActionUseScriptedHook's vtable 0x0126664C (defined here: the slot holds this body in 1 vtable(s), all of CTCActionUseScriptedHook or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts IsComponentSerialisable(void) at this slot; Ego_r/FableWin name set: ?IsComponentSerialisable@CTCActionUseScriptedHook@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCActionUseScriptedHook.h (opt-ins as in the landed row CWorldMap_GenerateRegionConnectivityGraph_00501450.cpp).
#define FABLE_RETAIL_CTCACTIONUSESCRIPTEDHOOK_VSLOTS
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CREGION
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_RETAIL_CAFILE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_SET
#define FABLE_RETAIL_CTHINGFILTER_HASTC_OUTOFLINE
#define FABLE_RETAIL_STLP_SET_EMPTY
#define FABLE_RETAIL_PLAYER
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "engine/CMapInfo.h"
#include "engine/CRegion.h"
#include "engine/CWorldMap.h"
#include "engine/CWorld.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_HasTC.h"
#include "engine/CThingFilter_HasTCOfLeafType.h"
#include "engine/CTCDRegionExit.h"
#include "engine/CTCActionUseScriptedHook.h"
#include "engine/CDiskFileWin32.h"
#include "fable_game.h"
bool CTCActionUseScriptedHook::IsComponentSerialisable() const
{
    return true;
}
