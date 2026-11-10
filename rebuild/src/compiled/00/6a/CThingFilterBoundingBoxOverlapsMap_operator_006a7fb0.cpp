// CThingFilter_BoundingBoxOverlapsMap::operator() -- test physics bounds against map area.
// Leaf: ??RCThingFilter_BoundingBoxOverlapsMap@@QBE_NABVCThing@@@Z
// Retail: Fable.exe 0x006A7FB0,220 bytes.
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_GETMAPBOX
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_COPY
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#define FABLE_RETAIL_C2DCOORDI_TOFLOAT
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_CORNERS
#define FABLE_RETAIL_C2DBOXF_CENTRE
#include "engine/C2DCoordI.h"
#include "engine/CThing.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CWorldMap.h"
#include "engine/C2DBoxF.h"
#include "engine/CThingFilter_BoundingBoxOverlapsMap.h"

bool CThingFilter_BoundingBoxOverlapsMap::operator()(const CThing& test_thing)const
{
    const CTCPhysicsBase* ptc_physics=test_thing.GetPhysicsTC();
    if(ptc_physics) {
        C3DVector centre;
        float radius;
        ptc_physics->GetBoundingInfo(centre,radius);
        C2DBoxI map_area_i=WorldMap.GetMapBox(MapNo);
        C2DBoxF map_area(map_area_i.GetTLPos().ToC2DCoordF(),map_area_i.GetBRPos().ToC2DCoordF());
        C2DBoxF bounding_box(C2DVector(centre),radius);
        if(map_area.TLX>bounding_box.BRX)return false;
        if(map_area.BRX<bounding_box.TLX)return false;
        if(map_area.TLY>bounding_box.BRY)return false;
        if(map_area.BRY<bounding_box.TLY)return false;
        return true;
    }
    return false;
}