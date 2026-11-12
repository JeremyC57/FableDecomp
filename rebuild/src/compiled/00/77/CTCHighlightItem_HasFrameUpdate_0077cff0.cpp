// CTCHighlightItem::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCHighlightItem@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0077CFF0, thiscall, `ret`.
// Identity: slot 8 of CTCHighlightItem's vtable 0x0126AF14 (defined here: the slot holds this body in 1 vtable(s), all of CTCHighlightItem or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCHighlightItem@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCHighlightItem.h (opt-ins as in the landed row CTCHighlightItem_DoMovement_0077c960.cpp).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_SETASMOVING
#define FABLE_RETAIL_CTCHIGHLIGHTITEM
#define FABLE_RETAIL_CHIGHLIGHTITEMDEF
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_HEIGHT_BELOW
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_MESH_BANK
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CMESHDATABANKENTRY
#define FABLE_RETAIL_CMESHDATABANK_PEEKINFO
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_C3DMESHINFO_BOUNDINGBOX
#define FABLE_RETAIL_C3DBOUNDINGBOX
#define FABLE_RETAIL_C3DBOUNDINGBOX_CENTER_MIN_FIRST
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_ROTATE
#define FABLE_RETAIL_C3DVECTOR
#include "fable_string.h"
#include "engine/CTCHighlightItem.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CHighlightItemDef.h"
#include "engine/CDefinitionManager.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CRightHandedSet.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/CMeshDataBank.h"
#include "engine/C3DMeshInfo.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include <math.h>
bool CTCHighlightItem::HasFrameUpdate() const
{
    return true;
}
