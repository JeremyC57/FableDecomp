// 006dffb0 behavior fixture: exact selected target pasted unchanged.
// Real constructed vector/set/interpolator/receiver objects; unused interfaces reduced.
// Graphic, water and engine methods are checked boundaries; no native layout/dispatch claim.
#include <stdio.h>
static int failures=0;static void Check(bool x){if(!x)++failures;}
static int events[24],eventCount;static void Event(int n){Check(eventCount<24);events[eventCount++]=n;}
class C3DVector{public:float X,Y,Z;C3DVector():X(0),Y(0),Z(0){}C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}C3DVector(const C3DVector&v):X(v.X),Y(v.Y),Z(v.Z){}};
static bool Same(const C3DVector&a,const C3DVector&b){return a.X==b.X&&a.Y==b.Y&&a.Z==b.Z;}
class CRightHandedSet{protected:C3DVector Up,Forward;public:CRightHandedSet():Up(),Forward(){}CRightHandedSet(const CRightHandedSet&r):Up(r.Up),Forward(r.Forward){}void Seed(float n){Up=C3DVector(n,n+1,n+2);Forward=C3DVector(n+3,n+4,n+5);}bool SameAs(const CRightHandedSet&r)const{return Same(Up,r.Up)&&Same(Forward,r.Forward);}};
class CCharString{public:int id;CCharString():id(0){}};
static int posConstructed,posDestroyed,orientationConstructed,orientationDestroyed;
class C3DPositionInterpolator{public:C3DVector StartPosition,EndPosition;C3DPositionInterpolator(const C3DVector&a,const C3DVector&b):StartPosition(a),EndPosition(b){++posConstructed;Event(50);}~C3DPositionInterpolator(){++posDestroyed;Event(52);}};
class C3DOrientationInterpolator{public:CRightHandedSet StartOrientation,EndOrientation;C3DOrientationInterpolator(const CRightHandedSet&a,const CRightHandedSet&b):StartOrientation(a),EndOrientation(b){++orientationConstructed;Event(60);}~C3DOrientationInterpolator(){++orientationDestroyed;Event(62);}};
class CTCPhysicsBase{public:CRightHandedSet orientation;C3DVector assigned;mutable int reads;int sets;CTCPhysicsBase():reads(0),sets(0){}virtual~CTCPhysicsBase(){}virtual const CRightHandedSet&GetRHSet()const{++reads;Event(10);return orientation;}virtual void SetPosition(const C3DVector&v){assigned=v;++sets;Event(40);}};
namespace NGraphicAppearance{class CTCGraphicAppearance{public:mutable int dummyCalls,helperCalls;bool result;CTCGraphicAppearance():dummyCalls(0),helperCalls(0),result(false){}bool GetDummyObject(const CCharString&name,long index,unsigned long zero,C3DVector&p,CRightHandedSet&r,float time)const{Check(name.id==77&&index==-3&&zero==0&&time==0.5f);++dummyCalls;p=C3DVector(4,5,6);r.Seed(40);Event(20);return result;}bool GetHelperPoint(const CCharString&name,long index,unsigned long zero,C3DVector&p,float time)const{Check(name.id==77&&index==-3&&zero==0&&time==0.5f);++helperCalls;p=C3DVector(7,8,9);Event(21);return result;}};}
class CThing{public:C3DVector position;CTCPhysicsBase*physics;NGraphicAppearance::CTCGraphicAppearance*graphic;mutable int posReads;CThing():position(1,2,3),physics(0),graphic(0),posReads(0){}const C3DVector&GetPos()const{++posReads;return position;}CTCPhysicsBase*GetPhysicsTC(){return physics;}NGraphicAppearance::CTCGraphicAppearance*GetTCGraphicAppearance(){return graphic;}};
namespace NParticleEngine{class CParticleEmitter{public:CRightHandedSet current,endOrientation,lastStartOrientation,lastEndOrientation;C3DVector endPos,lastStartPos,lastEndPos;mutable int currentReads,posReads,orientationReads;int positions,orientations;CParticleEmitter():endPos(-3,-2,-1),currentReads(0),posReads(0),orientationReads(0),positions(0),orientations(0){}CRightHandedSet GetCurrentOrientation()const{++currentReads;Event(11);return current;}const C3DVector&PeekEndPos()const{++posReads;return endPos;}const CRightHandedSet&PeekEndOrientation()const{++orientationReads;return endOrientation;}void SetPosition(const C3DPositionInterpolator&i){Check(posConstructed-posDestroyed==1);lastStartPos=i.StartPosition;lastEndPos=i.EndPosition;++positions;Event(51);}void SetOrientation(const C3DOrientationInterpolator&i){Check(orientationConstructed-orientationDestroyed==1);lastStartOrientation=i.StartOrientation;lastEndOrientation=i.EndOrientation;++orientations;Event(61);}};}
template<class T>class CIntelligentPointer{public:T*ptr;mutable int gets;CIntelligentPointer():ptr(0),gets(0){}T*GetPItem()const{++gets;return ptr;}};
template<class T>class CCountedPointer{public:T*Data;CCountedPointer():Data(0){}T*operator->()const{return Data;}};
class CWorldMap{public:float water;mutable int calls;mutable C3DVector query;CWorldMap():water(0),calls(0){}float GetWaterSizeZAt(const C3DVector&p)const{++calls;query=p;Event(30);return water;}};
class CTCBase{protected:CThing&ParentThing;const CWorldMap&world;CTCBase(CThing&t,const CWorldMap&w):ParentThing(t),world(w){}const CWorldMap&PeekWorldMap()const{return world;}};
class CTCDParticleEmitter:public CTCBase{public:class CParticleCreationParameters{public:CRightHandedSet RHSet;};CIntelligentPointer<NParticleEngine::CParticleEmitter>PEmitter;CIntelligentPointer<CThing>PAttachedToThing;unsigned long AttachmentFlags;CCharString AttachmentPosName;long AttachmentPosIndex;float FloatOffset;CCountedPointer<CParticleCreationParameters>PCreationParameters;bool WaitingForParticleToLoad,ResetEmitterPosition,ResetEmitterOrientation;
 CTCDParticleEmitter(CThing&t,const CWorldMap&w):CTCBase(t,w),AttachmentFlags(0),AttachmentPosIndex(-3),FloatOffset(-0.5f),WaitingForParticleToLoad(false),ResetEmitterPosition(false),ResetEmitterOrientation(false){AttachmentPosName.id=77;}void UpdateEmitterThingAttachment(bool,float);
};
void CTCDParticleEmitter::UpdateEmitterThingAttachment(bool interpolate, float time)
{
    if (!PEmitter.GetPItem() && !WaitingForParticleToLoad)
        return;
    C3DVector position = PAttachedToThing.GetPItem()->GetPos();
    CRightHandedSet orientation;
    if (PEmitter.GetPItem())
        orientation = (AttachmentFlags & 0x20) ? PAttachedToThing.GetPItem()->GetPhysicsTC()->GetRHSet()
                                               : PEmitter.GetPItem()->GetCurrentOrientation();
    else
        orientation = (AttachmentFlags & 0x20) ? PAttachedToThing.GetPItem()->GetPhysicsTC()->GetRHSet()
                                               : PCreationParameters->RHSet;
    if (!ResetEmitterPosition && !ResetEmitterOrientation)
        return;
    if (PAttachedToThing.GetPItem()->GetTCGraphicAppearance()) {
        if (AttachmentFlags & 2)
            PAttachedToThing.GetPItem()->GetTCGraphicAppearance()->GetDummyObject(AttachmentPosName, AttachmentPosIndex, 0, position, orientation, time);
        else if (AttachmentFlags & 4)
            PAttachedToThing.GetPItem()->GetTCGraphicAppearance()->GetHelperPoint(AttachmentPosName, AttachmentPosIndex, 0, position, time);
    }
    if (AttachmentFlags & 0x40) {
        float water = PeekWorldMap().GetWaterSizeZAt(position);
        if (water > 0.0f)
            position.Z = water;
    }
    if (AttachmentFlags & 0x80)
        position.Z += FloatOffset;
    if (ResetEmitterPosition) {
        ParentThing.GetPhysicsTC()->SetPosition(position);
        if (PEmitter.GetPItem()) {
            const C3DVector& start = interpolate ? position : PEmitter.GetPItem()->PeekEndPos();
            NParticleEngine::CParticleEmitter* emitter = PEmitter.GetPItem();
            emitter->SetPosition(C3DPositionInterpolator(start, position));
        }
    }
    if (ResetEmitterOrientation) {
        if (PEmitter.GetPItem()) {
            const CRightHandedSet& start = interpolate ? orientation : PEmitter.GetPItem()->PeekEndOrientation();
            NParticleEngine::CParticleEmitter* emitter = PEmitter.GetPItem();
            emitter->SetOrientation(C3DOrientationInterpolator(start, orientation));
        } else {
            PCreationParameters->RHSet = orientation;
        }
    }
}

int main(){unsigned int cases=0;const unsigned long bits[5]={2,4,0x20,0x40,0x80};
 for(int flags=0;flags<32;++flags)for(int loaded=0;loaded<2;++loaded)for(int waiting=0;waiting<2;++waiting)for(int resets=0;resets<4;++resets)for(int interpolate=0;interpolate<2;++interpolate)for(int graphicPresent=0;graphicPresent<2;++graphicPresent)for(int waterCase=0;waterCase<3;++waterCase){
  CTCPhysicsBase parentPhysics,attachedPhysics;attachedPhysics.orientation.Seed(10);CThing parent,attached;parent.physics=&parentPhysics;attached.physics=&attachedPhysics;NGraphicAppearance::CTCGraphicAppearance graphic;attached.graphic=graphicPresent?&graphic:0;graphic.result=(flags&1)!=0;NParticleEngine::CParticleEmitter particle;particle.current.Seed(20);particle.endOrientation.Seed(30);CTCDParticleEmitter::CParticleCreationParameters creation;creation.RHSet.Seed(50);CRightHandedSet initialCreation=creation.RHSet;CWorldMap world;world.water=waterCase==0?-1.0f:waterCase==1?0.0f:4.0f;CTCDParticleEmitter driver(parent,world);driver.PEmitter.ptr=loaded?&particle:0;driver.PAttachedToThing.ptr=&attached;driver.PCreationParameters.Data=&creation;driver.WaitingForParticleToLoad=waiting!=0;driver.ResetEmitterPosition=(resets&1)!=0;driver.ResetEmitterOrientation=(resets&2)!=0;for(int j=0;j<5;++j)if(flags&(1<<j))driver.AttachmentFlags|=bits[j];
  bool begun=loaded||waiting;bool active=begun&&resets;bool physicsOrientation=begun&&(driver.AttachmentFlags&0x20);bool currentOrientation=begun&&!physicsOrientation&&loaded;bool dummy=active&&graphicPresent&&(driver.AttachmentFlags&2);bool helper=active&&graphicPresent&&!dummy&&(driver.AttachmentFlags&4);bool water=active&&(driver.AttachmentFlags&0x40);
  C3DVector wanted=attached.position;CRightHandedSet wantedOrientation=physicsOrientation?attachedPhysics.orientation:loaded?particle.current:creation.RHSet;if(dummy){wanted=C3DVector(4,5,6);wantedOrientation.Seed(40);}else if(helper)wanted=C3DVector(7,8,9);C3DVector waterQuery=wanted;if(water&&world.water>0.0f)wanted.Z=world.water;if(active&&(driver.AttachmentFlags&0x80))wanted.Z-=0.5f;
  eventCount=posConstructed=posDestroyed=orientationConstructed=orientationDestroyed=0;driver.UpdateEmitterThingAttachment(interpolate!=0,0.5f);
  int expected[24],n=0;if(physicsOrientation)expected[n++]=10;else if(currentOrientation)expected[n++]=11;if(dummy)expected[n++]=20;else if(helper)expected[n++]=21;if(water)expected[n++]=30;if(active&&(resets&1)){expected[n++]=40;if(loaded){expected[n++]=50;expected[n++]=51;expected[n++]=52;}}if(active&&(resets&2)&&loaded){expected[n++]=60;expected[n++]=61;expected[n++]=62;}Check(n==eventCount);for(int j=0;j<n&&j<eventCount;++j)Check(events[j]==expected[j]);
  Check(attached.posReads==(begun?1:0));Check(attachedPhysics.reads==(physicsOrientation?1:0)&&particle.currentReads==(currentOrientation?1:0));Check(graphic.dummyCalls==(dummy?1:0)&&graphic.helperCalls==(helper?1:0));Check(world.calls==(water?1:0));if(water)Check(Same(world.query,waterQuery));
  bool setPos=active&&(resets&1),setOrientation=active&&(resets&2);Check(parentPhysics.sets==(setPos?1:0));if(setPos)Check(Same(parentPhysics.assigned,wanted));Check(particle.positions==(setPos&&loaded?1:0)&&particle.orientations==(setOrientation&&loaded?1:0));if(setPos&&loaded){Check(Same(particle.lastEndPos,wanted));Check(Same(particle.lastStartPos,interpolate?wanted:particle.endPos));}if(setOrientation&&loaded){Check(particle.lastEndOrientation.SameAs(wantedOrientation));Check(particle.lastStartOrientation.SameAs(interpolate?wantedOrientation:particle.endOrientation));}Check(creation.RHSet.SameAs(setOrientation&&!loaded?wantedOrientation:initialCreation));Check(particle.posReads==(setPos&&loaded&&!interpolate?1:0)&&particle.orientationReads==(setOrientation&&loaded&&!interpolate?1:0));Check(posConstructed==particle.positions&&posDestroyed==posConstructed&&orientationConstructed==particle.orientations&&orientationDestroyed==orientationConstructed);++cases;
 }
 Check(cases==6144);if(failures){fprintf(stderr,"failures=%d cases=%u\n",failures,cases);return 1;}puts("006dffb0_TEST PASS");return 0;
}