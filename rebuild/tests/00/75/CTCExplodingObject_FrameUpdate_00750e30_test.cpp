// Full production FrameUpdate and filter operator bodies; engine dependencies are explicit doubles.
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#ifndef _MSC_VER
#define FABLE_PORTABLE
#endif
#include "engine/GFFloatToLongCeil.h"
#ifdef _MSC_VER
#include <float.h>
#endif
struct C2DCoordI{long X,Y;C2DCoordI(long x=0,long y=0):X(x),Y(y){}};
struct C3DVector{float X,Y,Z;C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}C2DCoordI ToC2DCoordI()const{return C2DCoordI((long)X,(long)Y);}C3DVector operator-(const C3DVector&o)const{return C3DVector(X-o.X,Y-o.Y,Z-o.Z);}};
struct C2DBoxI{long TLX,TLY,BRX,BRY;C2DBoxI(const C2DCoordI&p,long r):TLX(p.X-r),TLY(p.Y-r),BRX(p.X+r),BRY(p.Y+r){}};
struct CCharString{std::string text;CCharString(const char*s):text(s){}};
struct CTCPhysicsBase{C3DVector pos;float Radius;CTCPhysicsBase():Radius(1){}C3DVector GetMapwhoPosition()const{return pos;}};
struct CThingSearchTools;struct CThing;
struct CTCBase{CThing*ParentThing;CTCBase():ParentThing(0){}CThing&GetParentThing(){return *ParentThing;}CThingSearchTools&GetThingSearchTools();};
struct CTCWobble:CTCBase{int calls;float seconds,angle;CTCWobble():calls(0),seconds(0),angle(0){}void SetWobbleParameters(float s,float a){++calls;seconds=s;angle=a;}};
enum {TCI_WOBBLE=247};
struct CThing{C3DVector pos;CTCPhysicsBase*physics;bool dead,hasWobble;int kills,adds,getPosCalls;CTCWobble wobble;CThing():pos(-2.75f,3.5f,7),physics(0),dead(false),hasWobble(false),kills(0),adds(0),getPosCalls(0){}const C3DVector&GetPos(){++getPosCalls;return pos;}const CTCPhysicsBase*GetPhysicsTC()const{return physics;}bool HasTC(int type){if(type!=TCI_WOBBLE)std::exit(90);return hasWobble;}CTCBase*AddTC(const CCharString&n,bool flag,const void*params){if(n.text!="CTCWobble"||flag||params)std::exit(91);++adds;hasWobble=true;return &wobble;}bool IsDead()const{return dead;}void Kill(bool immediate){if(!immediate)std::exit(92);++kills;dead=true;}};
struct CThingCreatureBase:CThing{};
struct CThingFilter_OverlappingCircle{C3DVector SpherePos;float Radius;CThingFilter_OverlappingCircle(const C3DVector&p,float r):SpherePos(p),Radius(r){}bool operator()(const CThing&thing)const;};
struct SearchRecord{C2DBoxI area;C3DVector center;float radius;SearchRecord(const C2DBoxI&a,const CThingFilter_OverlappingCircle&f):area(a),center(f.SpherePos),radius(f.Radius){}};
struct CThingSearchTools{static CThingSearchTools*active;bool inner,outer;std::vector<SearchRecord>records;CThingCreatureBase creature;CThingSearchTools(bool i,bool o):inner(i),outer(o){active=this;}template<class Filter,class T>bool GetPFirstThingInRoughArea(const C2DBoxI&a,const Filter&f,T**out){records.push_back(SearchRecord(a,f));bool result=records.size()==1?inner:outer;if(result)*out=&creature;return result;}};CThingSearchTools*CThingSearchTools::active=0;CThingSearchTools&CTCBase::GetThingSearchTools(){return *CThingSearchTools::active;}
template<class T>struct CIntelligentPointer{T*pointer;CIntelligentPointer(T*p=0):pointer(p){}T*operator->()const{return pointer;}bool operator!=(int)const{return pointer!=0;}};
template<class T>static bool IS_THING_ALIVE(const T&thing){return thing!=0&&!thing->IsDead();}
struct CExplodingObjectDef{long ProximityParticleEffect;};
struct CTCDParticleEmitter{static CThing created;static int calls;static long effect;static C3DVector position;static CThing*Create(long e,const C3DVector&p,bool f){if(f)std::exit(93);++calls;effect=e;position=p;return &created;}};CThing CTCDParticleEmitter::created;int CTCDParticleEmitter::calls=0;long CTCDParticleEmitter::effect=0;C3DVector CTCDParticleEmitter::position;
struct CTCExplodingObject:CTCBase{long CountdownTimer;bool TriggeredOnCreatureProximity;float TriggerRadius;CExplodingObjectDef*PDef;CIntelligentPointer<CThing>PProximityParticle;int explosions;CTCExplodingObject():CountdownTimer(0),TriggeredOnCreatureProximity(true),TriggerRadius(1.25f),PDef(0),explosions(0){}void Explode(){++explosions;}void FrameUpdate();};
inline bool CThingFilter_OverlappingCircle::operator()(const CThing& thing) const
{
    const CTCPhysicsBase* physics=thing.GetPhysicsTC();
    if(!physics)return false;
    C3DVector difference=physics->GetMapwhoPosition()-SpherePos;
    float radius=physics->Radius+Radius;
    float squared=difference.Y*difference.Y;
    squared+=difference.X*difference.X;
    return squared<radius*radius;
}
void CTCExplodingObject::FrameUpdate()
{
    if(CountdownTimer>0) {
        --CountdownTimer;
        if(CountdownTimer==0) { Explode();return; }
    }
    if(TriggeredOnCreatureProximity) {
        long radius=GFFloatToLongCeil(TriggerRadius+4.0f);
        C2DBoxI area(GetParentThing().GetPos().ToC2DCoordI(),radius);
        {
        CThingCreatureBase* creature;
        if(GetThingSearchTools().GetPFirstThingInRoughArea(area,CThingFilter_OverlappingCircle(GetParentThing().GetPos(),TriggerRadius),&creature)) {
            Explode();return;
        }
        if(GetThingSearchTools().GetPFirstThingInRoughArea(area,CThingFilter_OverlappingCircle(GetParentThing().GetPos(),TriggerRadius+4.0f),&creature)) {
            if(!GetParentThing().HasTC(TCI_WOBBLE)) {
                CTCWobble* wobble=static_cast<CTCWobble*>(GetParentThing().AddTC("CTCWobble",false,0));
                wobble->SetWobbleParameters(0.5f,1.0f/90.0f);
            }
            if(PDef->ProximityParticleEffect>0 && !IS_THING_ALIVE(PProximityParticle))
                PProximityParticle=CTCDParticleEmitter::Create(PDef->ProximityParticleEffect,GetParentThing().GetPos(),false);
        } else if(IS_THING_ALIVE(PProximityParticle)) {
            PProximityParticle->Kill(true);
        }
        }
    }
}

static void ck(bool ok,int line){if(!ok){std::printf("FAIL line %d\n",line);std::exit(1);}}
#define CHECK(x) ck((x),__LINE__)
static void run(long timer,bool trigger,bool inner,bool outer,long effect,int particleState,bool hasWobble){CThing owner,particle;owner.hasWobble=hasWobble;particle.dead=particleState==2;CThingSearchTools search(inner,outer);CExplodingObjectDef def;def.ProximityParticleEffect=effect;CTCExplodingObject x;x.ParentThing=&owner;x.PDef=&def;x.CountdownTimer=timer;x.TriggeredOnCreatureProximity=trigger;if(particleState)x.PProximityParticle=&particle;CTCDParticleEmitter::calls=0;x.FrameUpdate();CHECK(x.CountdownTimer==(timer>0?timer-1:timer));if(timer==1){CHECK(x.explosions==1&&search.records.empty()&&owner.adds==0&&particle.kills==0);return;}CHECK(x.explosions==(trigger&&inner?1:0));if(!trigger){CHECK(search.records.empty()&&owner.adds==0&&particle.kills==0);return;}CHECK(search.records.size()==(inner?1U:2U));for(unsigned int i=0;i<search.records.size();++i){const SearchRecord&r=search.records[i];CHECK(r.area.TLX==-8&&r.area.TLY==-3&&r.area.BRX==4&&r.area.BRY==9);CHECK(r.center.X==-2.75f&&r.center.Y==3.5f&&r.center.Z==7);CHECK(r.radius==(i?5.25f:1.25f));}if(inner){CHECK(owner.adds==0&&CTCDParticleEmitter::calls==0&&particle.kills==0);return;}if(outer){CHECK(owner.adds==(hasWobble?0:1));CHECK(owner.wobble.calls==(hasWobble?0:1));if(!hasWobble)CHECK(owner.wobble.seconds==.5f&&owner.wobble.angle==1.0f/90.0f);bool create=effect>0&&particleState!=1;CHECK(CTCDParticleEmitter::calls==(create?1:0));if(create){CHECK(x.PProximityParticle.pointer==&CTCDParticleEmitter::created);CHECK(CTCDParticleEmitter::effect==effect&&CTCDParticleEmitter::position.X==owner.pos.X&&CTCDParticleEmitter::position.Y==owner.pos.Y&&CTCDParticleEmitter::position.Z==owner.pos.Z);}CHECK(particle.kills==0);}else{CHECK(owner.adds==0&&CTCDParticleEmitter::calls==0);CHECK(particle.kills==(particleState==1?1:0));}}
int main(){
#ifdef _MSC_VER
_controlfp(_PC_53|_RC_NEAR|_MCW_EM,_MCW_PC|_MCW_RC|_MCW_EM);
#endif
run(1,true,false,true,5,1,false);run(2,false,false,false,5,1,false);run(-1,false,false,false,5,1,false);run(0,true,true,true,5,1,false);run(0,true,false,true,5,0,false);run(2,true,false,true,5,2,true);run(0,true,false,true,5,1,true);run(0,true,false,true,0,0,false);run(0,true,false,true,-1,1,true);run(0,true,false,false,5,1,false);run(0,true,false,false,5,0,false);run(0,true,false,false,5,2,false);CThing t;CTCPhysicsBase p;t.physics=&p;p.pos=C3DVector(2,0,99);CThingFilter_OverlappingCircle f(C3DVector(0,0,0),1);CHECK(!f(t));p.pos.X=1.5f;CHECK(f(t));t.physics=0;CHECK(!f(t));std::puts("00750e30_TEST PASS");return 0;}