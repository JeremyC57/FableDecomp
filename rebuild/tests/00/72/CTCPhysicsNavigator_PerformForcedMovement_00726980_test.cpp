// 00726980 CTCPhysicsNavigator::PerformForcedMovement
// Leaf: ?PerformForcedMovement@CTCPhysicsNavigator@@QAEXAAV?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@ABVC3DVector@@AAV4@@Z
// Flags:
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSSTANDARD
#define FABLE_RETAIL_CTCPHYSICSSTANDARD_MOVE
#define FABLE_RETAIL_CTCPHYSICSSTANDARD_IMPULSE
#define FABLE_RETAIL_CTCPHYSICSNAVIGATOR
#define FABLE_RETAIL_CTCPHYSICSNAVIGATOR_MOVE_PASS27
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_C3DVECTOR_ISNONZERO_ORIGINAL_YXZ
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#define FABLE_RETAIL_C2DVECTOR_ORIGINAL_COPIES_PASS26
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_LOCALS_XYZ
#define FABLE_RETAIL_CTCPHYSICSBASE_IMPULSE
#define FABLE_RETAIL_CPHYSICSCOLLIDERESPONSE_CTOR
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "fable_assert.h"
#include "engine/CThing.h"
#include "engine/CPhysicsCollideResponse.h"
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/CWorld.h"
#include "engine/CTCPhysicsNavigator.h"
#include "engine/GFSqr.h"


// FableWin's debug body loads this exact read-only public for its speed test.
extern const float DELTA_NOTIONAL_ZERO;

void CTCPhysicsNavigator::PerformForcedMovement(std::vector<CThing*>& thing_list, const C3DVector& impulse, C3DVector& current_pos)
{
    bool hit_ground = false;
    if (!thing_list.empty() || CanCollideWithLandscape()) {
        float speed = impulse.GetMagnitude();
        C3DVector velocity = impulse;
        C3DVector velocity_step = impulse;
        CPhysicsCollideResponse response;
        long count = 0;
        long no_interaction_steps = static_cast<long>(speed * 12.0f);
        if (no_interaction_steps < 1) no_interaction_steps = 1;
        velocity_step /= static_cast<float>(no_interaction_steps);
        bool still_active = speed > DELTA_NOTIONAL_ZERO;
        while (still_active) {
            UpdateVelocityForImpassability(current_pos, velocity_step);
            C3DVector desired_pos = current_pos + velocity_step;
            bool collided = false;
            std::vector<CThing*>::iterator test_thing_it;
            for (test_thing_it = thing_list.begin(); test_thing_it != thing_list.end(); ++test_thing_it) {
                CThing& test_thing = **test_thing_it;
                CTCPhysicsBase& physics_tc = *test_thing.GetPhysicsTC();
                if (HasCollidedWithThingFromTo(physics_tc,current_pos,desired_pos,response)) {
                    if (IsCollisionWithThingToCauseResponse(physics_tc)) {
                        if (physics_tc.IsPhysicsMeshTypeCylinder()) {
                            collided = ImpulseRespondToCollisonWithCylinder(response,velocity_step);
                        } else {
                            collided = true;
                            ModifyResponseForNearVerticalSlopes(response);
                            ImpulseRespondToCollision(response,velocity_step);
                        }
                    }
                    UpdateCollisionGameInteractions(test_thing,response);
                }
            }
            if (CanCollideWithLandscape() && HasCollidedWithLandscapeAt(desired_pos,response)) {
                desired_pos = response.CollidePos;
                ImpulseRespondToCollision(response,velocity_step);
                hit_ground = true;
            }
            ++count;
            if (collided) {
                if (count == 1 && no_interaction_steps == 1) no_interaction_steps = 3;
            } else {
                current_pos = desired_pos;
            }
            if (count >= no_interaction_steps) still_active = false;
        }
    }
    if (hit_ground) UpdateCollisionGroundIneractions(current_pos);
}

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
const float DELTA_NOTIONAL_ZERO = 9.9999997473787516e-05f;
template<class T> union Storage {
    double aligned; unsigned char bytes[sizeof(T)];
    T* pointer(){return reinterpret_cast<T*>(bytes);}
};
static Storage<CTCPhysicsNavigator> actor;
static Storage<CTCPhysicsBase> other;
static Storage<CThing> item;
static int cases, steps, thingTests, environmentTests, interactions, impulses, slopes, cylinders, grounds;
static bool landEnabled, landHit, objectHit, responds, cylinder, cylinderStops;

void CTCPhysicsStandard::UpdateVelocityForImpassability(const C3DVector&,C3DVector&) { assert(this==actor.pointer());++steps; }
bool CTCPhysicsStandard::IsCollisionWithThingToCauseResponse(const CTCPhysicsBase& p)const {assert(&p==other.pointer());return responds;}
bool CTCPhysicsBase::IsPhysicsMeshTypeCylinder()const {assert(this==other.pointer());return cylinder;}
bool CTCPhysicsBase::ImpulseRespondToCollisonWithCylinder(const CPhysicsCollideResponse& r,C3DVector&)const {
    assert(this==actor.pointer() && r.Friction==1.0f);++cylinders;return cylinderStops;
}
void CTCPhysicsBase::ModifyResponseForNearVerticalSlopes(CPhysicsCollideResponse& r)const {assert(r.Friction==1.0f);++slopes;}
void CTCPhysicsBase::ImpulseRespondToCollision(const CPhysicsCollideResponse&,C3DVector&)const {++impulses;}
void CTCPhysicsStandard::UpdateCollisionGameInteractions(CThing& t,const CPhysicsCollideResponse&) {assert(&t==item.pointer());++interactions;}
void CTCPhysicsBase::UpdateCollisionGroundIneractions(const C3DVector&) {++grounds;}

class Trace {
public:
    virtual bool CanLand()const {return landEnabled;}
    virtual bool Object(const CTCPhysicsBase& p,const C3DVector& from,const C3DVector& to,CPhysicsCollideResponse& r)const {
        assert(&p==other.pointer() && r.Friction==1.0f && to.X>=from.X);++thingTests;return objectHit;
    }
    virtual bool Land(const C3DVector&,CPhysicsCollideResponse& r)const {
        ++environmentTests;r.CollidePos=C3DVector(16.0f,2.0f,4.0f);return landHit;
    }
};
int main() {
    Trace trace;void** methods;memcpy(&methods,&trace,sizeof(methods));
    void* table[93]={0};table[59]=methods[0];table[90]=methods[1];table[92]=methods[2];
    void** vtable=table;memcpy(actor.bytes,&vtable,sizeof(vtable));
    CTCPhysicsBase* op=other.pointer();memcpy(item.bytes+0x60,&op,sizeof(op));
    CThing* t=item.pointer();CThing** first=&t;
    Storage<std::vector<CThing*> > list;memset(list.bytes,0,sizeof(list.bytes));
    memcpy(list.bytes,&first,sizeof(first));
    const float speeds[]={0.0f,DELTA_NOTIONAL_ZERO,0.03125f,0.25f};
    for(int f=0;f<128;++f)for(int i=0;i<4;++i){
        bool populated=(f&1)!=0;landEnabled=(f&2)!=0;landHit=(f&4)!=0;
        objectHit=(f&8)!=0;responds=(f&16)!=0;cylinder=(f&32)!=0;cylinderStops=(f&64)!=0;
        CThing** last=first+(populated?1:0);memcpy(list.bytes+sizeof(first),&last,sizeof(last));
        steps=thingTests=environmentTests=interactions=impulses=slopes=cylinders=grounds=0;
        C3DVector pos(0,0,0), impulse(speeds[i],0,0);
        actor.pointer()->PerformForcedMovement(*list.pointer(),impulse,pos);
        const bool active=(populated||landEnabled) && i>=2;
        const bool blocked=populated && objectHit && responds && (!cylinder || cylinderStops);
        const int count=active?(i==3 || blocked?3:1):0;
        assert(steps==count && thingTests==count*populated && environmentTests==count*landEnabled);
        assert(interactions==count*(populated&&objectHit));
        assert(slopes==count*(populated&&objectHit&&responds&&!cylinder));
        assert(cylinders==count*(populated&&objectHit&&responds&&cylinder));
        assert(impulses==slopes+count*(landEnabled&&landHit));
        assert(grounds==(active&&landEnabled&&landHit));
        if(!active||blocked)assert(pos.X==0 && pos.Y==0 && pos.Z==0);
        else if(landEnabled&&landHit)assert(pos.X==16 && pos.Y==2 && pos.Z==4);
        else assert(fabs(pos.X-speeds[i])<1.0e-6 && pos.Y==0 && pos.Z==0);
        ++cases;
    }
    assert(cases==512);puts("00726980_TEST PASS");return 0;
}