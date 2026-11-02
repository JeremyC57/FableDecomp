// NCreatureHiding::GetValidPositions(vector<C3DVector>&,const CWorldMap&,const CThingSearchTools&,const C3DVector&,float,bool,const CTCShapeManager*) -- gather usable or hidden radial positions.
// Leaf: ?GetValidPositions@NCreatureHiding@@YIXAAV?$vector@VC3DVector@@V?$allocator@VC3DVector@@@std@@@std@@ABVCWorldMap@@ABVCThingSearchTools@@ABVC3DVector@@M_NPBVCTCShapeManager@@@Z
#define FABLE_RETAIL_VALIDPOSITIONS
#define FABLE_STLPORT_CREATURE_POSITIONS
#define FABLE_STLPORT_VISIBLE_POSITIONS_OVERFLOW
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#include <math.h>
#pragma intrinsic(cos,sin)
#include "engine/CThing.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CPhysicsMeshObject.h"
#include "engine/CMeshIntersectionData.h"
#include "engine/CWorldMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_None.h"
#include "engine/CTCShapeManager.h"
#include "engine/C2DBoxI.h"
#include "engine/C2DCoordI.h"
extern const double TWO_PI;
extern const float DELTA_NOTIONAL_ZERO;
namespace NCreatureHiding {
void __fastcall GetValidPositions(std::vector<C3DVector>& positions,const CWorldMap& wmap,
 const CThingSearchTools& search_tools,const C3DVector& about_position,float radius,
 bool get_hidden_positions,const CTCShapeManager* pExclusionArea)
{
    positions.clear();
    const float d_rad=2.0f<(0.5f>radius*0.125f?0.5f:radius*0.125f)?2.0f:(0.5f>radius*0.125f?0.5f:radius*0.125f);
    std::vector<const CThing*> thing_list;
    search_tools.GetAllThingsInRoughArea<const CThing>(C2DBoxI(about_position.ToC2DCoordI(),(long)radius),CThingFilter_None(),thing_list);
    for(float rad=0.0f;rad<radius+DELTA_NOTIONAL_ZERO;rad+=d_rad) {
        long n_thet=(long)(rad*2.0f+1.0f);
        for(long c1=0;c1<n_thet;++c1) {
            float thet=((float)c1/(float)n_thet)*TWO_PI;
            C3DVector r=C3DVector((float)cos(thet),(float)sin(thet),0.0f)*rad+about_position;
            if(!wmap.IsPosInMap(r)) continue;
            bool got_collision=false;
            std::vector<const CThing*>::iterator it=thing_list.begin();
            while(!got_collision && it!=thing_list.end()) {
                const CThing* pthing=*it++;
                if(pthing && pthing->GetPhysicsTC()->GetPhysicsMeshInfo()) {
                CMeshIntersectionData int_dat;
                if(pthing->GetPhysicsTC()->GetPhysicsMeshObject().GetPointIntersectionFromTo(
                    C3DVector(r.X,r.Y,r.Z+5.0f),C3DVector(r.X,r.Y,r.Z-5.0f),int_dat)) {
                    got_collision=true;
                    r=int_dat.IntersectionPosition;
                    r.Z+=0.1f;
                }
                }
            }
            float ground_height=wmap.GetGroundSizeZAt(r);
            if(!got_collision || ground_height>r.Z) r.Z=ground_height;
            if(pExclusionArea && pExclusionArea->IsPosInAnyShape(r)) continue;
            bool hidable=get_hidden_positions && !wmap.IsPosPassable(r.ToC2DCoordI());
            bool usable=!get_hidden_positions && wmap.IsPosPassable(r.ToC2DCoordI());
            if(hidable&&!got_collision) positions.push_back(r);
            else if(usable) positions.push_back(r);
        }
    }
}
}
