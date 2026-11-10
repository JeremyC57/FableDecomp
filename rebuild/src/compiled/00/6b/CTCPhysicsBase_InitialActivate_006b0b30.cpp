// CTCPhysicsBase::InitialActivate() -- place an eligible physics object in the map index
// Retail: 0x006B0B30,217 bytes,virtual thiscall
// Leaf: ?InitialActivate@CTCPhysicsBase@@UAEXXZ
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_MAPWHOPOSITION
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_STATTYPE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_LOADSINGLE
#define FABLE_RETAIL_CTHING_ISPLACED
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ADD2D
#define FABLE_RETAIL_C2DVECTOR
#include "engine/CThing.h"
#include "engine/CTCMapwho.h"
#include "engine/CTCPhysicsBase.h"
void CTCPhysicsBase::InitialActivate(){
    if(GetParentThing().IsToBePlacedInMap()){
        CTCMapwho* pmapwho=0;
        GetParentThing().GetTC(&pmapwho);
        C3DVector mapwho_pos=GetMapwhoPosition();
        if(pmapwho->IsToBePlacedInMapwho(GetMapwhoPosition()))
            pmapwho->PlaceInMapwho(GetMapwhoPosition());
    }
}