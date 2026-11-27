#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <math.h>
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D_ORIGINAL_SUM_PASS23
#define FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS
#include "engine/C3DVector.h"
#include "engine/C2DBoxI.h"
#include "engine/CFilter_Bind2.h"
#define FABLE_RETAIL_IS_THING_ALIVE
#include "engine/ThingAlive.h"
#include "fable_assert.h"
static void check(bool ok) { if (!ok) exit(2); }
struct CThing {
    C3DVector pos;
    bool dead;
    CThing() : pos(0,0,0), dead(false) {}
    const C3DVector& GetPos() const { return pos; }
    bool IsDead() const { return dead; }
};
struct CThingCreatureBase : CThing {};
struct CThingFilter_IsNot {
    const CThing& excluded;
    CThingFilter_IsNot(const CThing& thing) : excluded(thing) {}
    bool operator()(const CThing& thing) const { return &thing != &excluded; }
};
static CThingCreatureBase actor, target;
static CThing obstacle;
static bool query_result, manager_result, include_obstacle;
static int searches, camera_requests, rays;
static long center_x, center_y, radius;
struct SceneSearch {
    template<class T,class F> void GetAllThingsInRoughArea(const C2DBoxI& box,const F& filter,std::vector<T*>& output) const {
        ++searches;
        check(output.empty() && !filter(actor) && !filter(target) && filter(obstacle));
        check(box.TLX == center_x-radius && box.BRX == center_x+radius);
        check(box.TLY == center_y-radius && box.BRY == center_y+radius);
        if (include_obstacle) output.push_back(&obstacle);
    }
};
namespace NCameraModes {
struct CTools {
    enum ELOSTest { TEST_ALL_BUT_CREATURES=7 };
    bool IsLineOfSight(const std::vector<const CThing*>& items,const C3DVector& from,const C3DVector& to,
        C3DVector& point,C3DVector& normal,float& hit_radius,const CThing** hit,ELOSTest mask,bool flag) const {
        ++rays;
        check(searches==1 && camera_requests==1 && mask==7 && !flag);
        check(&from==&actor.pos && &to!=&target.pos && to.X==target.pos.X && to.Y==target.pos.Y && to.Z==target.pos.Z+1);
        check(items.size()==(include_obstacle?1:0));
        if(include_obstacle) check(items[0]==&obstacle);
        check(&point!=&normal); point=to; normal=from; hit_radius=2; *hit=&obstacle;
        return query_result;
    }
};
}
struct CGameCameraManager {
    NCameraModes::CTools tools;
    const NCameraModes::CTools& GetCameraTools() const { return tools; }
};
struct CameraFixture {
    CGameCameraManager manager;
    bool GetCameraManager(const CGameCameraManager** result) const {
        ++camera_requests; *result=&manager; return manager_result;
    }
};
struct WorldFixture {
    SceneSearch search;
    CameraFixture camera;
    const SceneSearch& PeekThingSearchTools() const { return search; }
    const CameraFixture* PeekGameCamera() const { return &camera; }
};
struct CAIStateGroup_NymphAttack {
    WorldFixture world;
    CThingCreatureBase& GetCreature(){return actor;}
    const WorldFixture& GetWorld()const{return world;}
    bool IsTargetInLineOfSight(CThingCreatureBase*);
};

bool CAIStateGroup_NymphAttack::IsTargetInLineOfSight(CThingCreatureBase* pTarget)
{
    if (IS_THING_ALIVE(pTarget)) {
        C3DVector targPos = pTarget->GetPos();
        std::vector<const CThing*> things;
        C3DVector midPoint = (GetCreature().GetPos() + targPos) * 0.5f;
        C3DVector delta = targPos - midPoint;
        float rad = (delta.GetMagnitude2D()) * 1.1f;
        GetWorld().PeekThingSearchTools().GetAllThingsInRoughArea<const CThing>(
            C2DBoxI(midPoint.ToC2DCoordI(), static_cast<long>(rad)),
            GFFilter_Bind2(CThingFilter_IsNot(GetCreature()), CThingFilter_IsNot(*pTarget)), things);
        const CGameCameraManager* pCamMan;
        if (!GetWorld().PeekGameCamera()->GetCameraManager(&pCamMan)) {
            FABLE_RETAIL_ASSERT_ARM(false);
        }
        C3DVector int_pos, int_normal;
        const CThing* phit_thing;
        float cam_rad;
        targPos.Z += 1.0f;
        return pCamMan->GetCameraTools().IsLineOfSight(things, GetCreature().GetPos(), targPos,
            int_pos, int_normal, cam_rad, &phit_thing, NCameraModes::CTools::TEST_ALL_BUT_CREATURES, false);
    }
    return false;
}

int main() {
    CAIStateGroup_NymphAttack state;
    const float offset[][2]={{0,0},{3,4},{-3,4},{-8,-6},{2.25f,-7.75f},{-15.5f,8.25f}};
    for(int px=-2;px<=2;++px) for(int py=-2;py<=2;++py) for(int k=0;k<6;++k)
    for(int alive=0;alive<3;++alive) for(int flags=0;flags<8;++flags) {
        actor.pos=C3DVector(px*7.25f,py*5.5f,px*16.0f);
        target.pos=C3DVector(actor.pos.X+offset[k][0],actor.pos.Y+offset[k][1],py*32.0f);
        const float before=target.pos.Z;
        center_x=(long)((double(actor.pos.X)+target.pos.X)*0.5);
        center_y=(long)((double(actor.pos.Y)+target.pos.Y)*0.5);
        radius=(long)(sqrt(double(offset[k][0])*offset[k][0]+double(offset[k][1])*offset[k][1])*0.55);
        target.dead=alive==1; manager_result=(flags&1)!=0;
        include_obstacle=(flags&2)!=0; query_result=(flags&4)!=0;
        searches=camera_requests=rays=0;
        bool actual=state.IsTargetInLineOfSight(alive?&target:0);
        check(actual==(alive==2 && query_result));
        check(searches==(alive==2) && camera_requests==searches && rays==searches);
        check(target.pos.Z==before);
    }
    puts("0091c220_TEST PASS");
    return 0;
}