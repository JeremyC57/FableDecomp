#define FABLE_RETAIL_CMINIMAPDISPLAY
#define FABLE_RETAIL_CMINIMAPDISPLAY_DRAWMARKERS
#define FABLE_RETAIL_CMINIMAPDISPLAY_GETDRAWPOSITION
#define FABLE_RETAIL_CMINIMAPDISPLAY_RELATIVEMAPSIZE_BODY
#define FABLE_RETAIL_C2DVECTOR_INITIALISE
#define FABLE_RETAIL_CREGIONMINIMAP
#define FABLE_RETAIL_CREGIONMINIMAP_EXTENTS_NOINLINE
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_COPY
#define FABLE_RETAIL_C2DBOXF_GETEXTENTS
#define FABLE_RETAIL_C2DBOXF_EXTENT_NAMED
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CREGION
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETWORLD
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/GFFloatToLongFloor.h"
#include "engine/C2DVector.h"
#include "engine/C3DVector.h"
#include "engine/C2DCoordI.h"
#include "engine/C2DExtentsI.h"
#include "engine/C2DExtentsF.h"
#include "engine/C2DBoxI.h"
#include "engine/C2DBoxF.h"
#include "engine/CRegionMinimap.h"
#include "engine/CRegion.h"
#include "engine/CWorldMap.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerGui.h"
#include "engine/CMiniMapDisplay.h"


typedef void (CMiniMapDisplay::*TPos)(const C3DVector&, C2DVector&) const;
typedef void (CMiniMapDisplay::*TSize)(C2DVector&) const;
TPos HelperPos = &CMiniMapDisplay::GetRelativePosOnMap;
TSize HelperSize = &CMiniMapDisplay::GetRelativeMapSize;
