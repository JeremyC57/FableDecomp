// CTCTeleporter::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCTeleporter@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E0520, thiscall, `ret`.
// Identity: slot 8 of CTCTeleporter's vtable 0x0126FDA4 (defined here: the slot holds this body in 1 vtable(s), all of CTCTeleporter or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCTeleporter@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCTeleporter.h (opt-ins as in the landed row CTCInventoryMap_GetRegionTeleporter_005fbea0.cpp).
#define FABLE_RETAIL_CTCTELEPORTER_VSLOTS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FIRSTTHING
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FIRSTTHING_BODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CREGION
#define FABLE_RETAIL_CTCINVENTORYMAP_TELEPORT
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
#include "engine/CTCInventoryMap.h"
#include "engine/CThing.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/CRegion.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_HasTC.h"
#include "engine/CThingFilter_InMap.h"
#include "engine/CFilter_Bind2.h"
#include "engine/CTCTeleporter.h"
bool CTCTeleporter::HasFrameUpdate() const
{
    return true;
}
