#include <stdio.h>
#include <math.h>
#include <list>
#include <map>
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ADD2D
#define FABLE_RETAIL_C3DVECTOR_ADD2D_DIRECT
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE_SQRTF
#define FABLE_RETAIL_AVOID_REGION_FRAME_PASS29
#include "engine/C2DVector.h"
struct CWorld {static long frame;static long GetFrame(){return frame;}};long CWorld::frame;
struct CTCAIScratchpad {float offset;void SetTargetingDistanceOffset(float d){offset=d;}};
struct CTCDRegionExit {float r;float GetRadius()const{return r;}};
struct RH {C3DVector GetRight()const{return C3DVector(1,0,0);}C3DVector GetForward()const{return C3DVector(0,1,0);}};
struct Physics {RH GetRHSet()const{return RH();}};
struct CThing {
    C3DVector pos;CTCAIScratchpad scratchpad;CTCDRegionExit exit;bool hasScratch,isExit,dead;Physics physics;
    CThing():pos(0,0,0),hasScratch(false),isExit(false),dead(false){exit.r=5;scratchpad.offset=-1;}
    const C3DVector& GetPos()const{return pos;}bool IsDead()const{return dead;}Physics* GetPhysicsTC(){return &physics;}
    bool GetTC(CTCAIScratchpad** p){*p=hasScratch?&scratchpad:0;return hasScratch;}
    bool GetTC(CTCDRegionExit** p){*p=isExit?&exit:0;return isExit;}
};
template<class T>struct CIntelligentPointer {T* p;CIntelligentPointer(T* a=0):p(a){}T* operator->()const{return p;}bool operator==(int)const{return p==0;}};
template<class T>bool IS_THING_DEAD(const T& t){return !t.p || t.p->dead;}
struct CTCAvoidRegionExit {
    CThing me; CIntelligentPointer<CThing> EntranceMarker,ExitMarker;
    unsigned long NextFrameCheck;bool MarkersAcquired;long FrameLastUpdated;int updates,best;
    std::list<CIntelligentPointer<CThing> > ExitMarkers,EntranceMarkers;std::map<CThing*,float> DistMap;
    CTCAvoidRegionExit():NextFrameCheck(0),MarkersAcquired(false),FrameLastUpdated(-1),updates(0),best(0){}
    void UpdateMarkers(){++updates;}void UpdateBestDist(){++best;}CThing& GetParentThing(){return me;}
    void FrameUpdate();
};
template<> inline bool IS_THING_DEAD(const CIntelligentPointer<CThing>& thing)
{
    if (!(thing == 0)) {
        if (!thing->IsDead()) return false;
    }
    return true;
}
void CTCAvoidRegionExit::FrameUpdate()
{
    if (!MarkersAcquired) {MarkersAcquired=true; UpdateMarkers();}
    if (FrameLastUpdated!=CWorld::GetFrame()) {FrameLastUpdated=CWorld::GetFrame();UpdateBestDist();}
    if (NextFrameCheck>(unsigned long)CWorld::GetFrame()) return;
    NextFrameCheck=CWorld::GetFrame()+5;
    CTCAIScratchpad* scratchpad=0;
    if(GetParentThing().GetTC(&scratchpad)) scratchpad->SetTargetingDistanceOffset(0.0f);
    DistMap.erase(&GetParentThing());
    if(!ExitMarkers.size() || !EntranceMarkers.size()) return;
    float dist=0.0f;
    ExitMarker=CIntelligentPointer<CThing>(0); EntranceMarker=CIntelligentPointer<CThing>(0);
    if (false) {} else for(std::list<CIntelligentPointer<CThing> >::iterator it=ExitMarkers.begin();it!=ExitMarkers.end();++it) {
        if(IS_THING_DEAD(*it)){UpdateMarkers();return;}
        CTCDRegionExit* exit;
        if(!(*it)->GetTC(&exit))continue;
        float radius=exit->GetRadius(); CThing& parent=GetParentThing();
        if(!((parent.GetPos()-(*it)->GetPos()).GetMagnitude()<radius*2.0f))continue;
        C2DVector right((*it)->GetPhysicsTC()->GetRHSet().GetRight()*radius);
        C2DVector front((*it)->GetPos()+right),back((*it)->GetPos()-right);
        C2DVector pos(GetParentThing().GetPos());
        if((pos-back).Dot(right)>0.0f && (pos-front).Dot(right)<0.0f){ const CIntelligentPointer<CThing>& pick=*it;
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
 ExitMarker=pick;
#ifdef _MSC_VER
#pragma inline_depth(8)
#endif
}
    }
    if(ExitMarker==0)return;
    C2DVector forward(ExitMarker->GetPhysicsTC()->GetRHSet().GetForward());
    CThing& parent=GetParentThing(); if(!(C2DVector(parent.GetPos()-ExitMarker->GetPos()).Dot(forward)>0.0f)) return;
    if (false) {} else for(std::list<CIntelligentPointer<CThing> >::iterator it=EntranceMarkers.begin();it!=EntranceMarkers.end();++it) {
        if(IS_THING_DEAD(*it)){UpdateMarkers();return;}
        CThing* thing=(*it).operator->(); float candidate=(thing->GetPos()-ExitMarker->GetPos()).GetSquaredMagnitude();
        if(EntranceMarker==0 || dist>candidate){EntranceMarker=*it;dist=candidate;}
    }
    dist=GFRoot(dist);
    if(scratchpad)scratchpad->SetTargetingDistanceOffset(dist);
    DistMap[&GetParentThing()]=dist;
}
int main(){
 for(int i=0;i<8192;++i){
    CTCAvoidRegionExit c; CThing e,t1,t2;
    c.me.hasScratch=(i&1)!=0;c.me.pos=C3DVector(0,(i&4096)?-1:1,0);e.isExit=(i&2)!=0;
    e.dead=(i&4)!=0;t1.dead=(i&8)!=0;
    float d1=(i&128)?5.0f:1.0f;
    float d2=(i&256)?1.0f:5.0f;
    t1.pos=C3DVector(d1,0,0);t2.pos=C3DVector(0,0,d2);
    if(i&512)c.me.pos=C3DVector(5,-1,0);
    if(i&1024)c.me.pos=C3DVector(-5,-1,0);
    if(i&2048)c.me.pos=C3DVector(0,0,0);
    if(i&16)c.ExitMarkers.push_back(&e);if(i&32)c.EntranceMarkers.push_back(&t1);
    if(i&64)c.EntranceMarkers.push_back(&t2);
    c.DistMap[&c.me]=999.0f; c.DistMap[&t1]=31.0f; c.DistMap[&t2]=32.0f;
    CWorld::frame=i;c.FrameUpdate();bool lists=(i&16)!=0 && (i&(32|64))!=0;
    bool valid=lists && !(i&(512|1024|2048|4096)) && !e.dead && e.isExit && !((i&32)&&t1.dead);
    if(c.DistMap.size()!=(valid?3:2) || c.DistMap[&t1]!=31.0f || c.DistMap[&t2]!=32.0f)return 1;
    bool first=(i&32) && (!(i&64) || d1<=d2);float expected=first?d1:d2;
    if(valid){if(c.DistMap[&c.me]!=expected || c.EntranceMarker.p!=(first?&t1:&t2))return 2;}
    if(c.me.hasScratch && c.me.scratchpad.offset!=(valid?expected:0.0f))return 3;
    int u=c.updates;c.FrameUpdate();if(c.updates!=u || c.best!=1 || c.NextFrameCheck!=(unsigned long)i+5)return 4;
    CWorld::frame=i+1;c.FrameUpdate();if(c.best!=2 || c.updates!=u)return 5;
 }puts("007e9460_TEST PASS");return 0;
}