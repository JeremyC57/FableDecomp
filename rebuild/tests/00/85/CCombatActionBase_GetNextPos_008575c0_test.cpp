#include <stdio.h>
#include <stdlib.h>
#include "fable_assert.h"
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR
#define FABLE_RETAIL_CMATRIX3X4_BILLBOARD
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE
#include "engine/C3DVector.h"
#include "engine/CMatrix3x4.h"
#include "engine/CRightHandedSet.h"
namespace MovementTest {
void check(bool ok){if(!ok)exit(1);}
bool same(const C3DVector& v,float x,float y,float z){return v.X==x&&v.Y==y&&v.Z==z;}
struct Case {
 bool rotate,flourish;int duration,entry,bank,delta,rhs,set,angle,face,peek,changes,delays;float expectedLast,expectedCurrent;
 Case():rotate(false),flourish(false),duration(0),entry(0),bank(0),delta(0),rhs(0),set(0),angle(0),face(0),peek(0),changes(0),delays(0),expectedLast(0),expectedCurrent(0){}
} *state;
struct CMainGameComponent{static long GetConstantFPS(){return 4;}};
struct CAnimationEntry{};
template<class T>struct CCountedPointer{T* p;explicit CCountedPointer(T* q):p(q){}T* Get()const{return p;}};
void matrix(CMatrix3x4& m){m.E11=0;m.E12=1;m.E13=0;m.E21=-1;m.E22=0;m.E23=0;m.E31=0;m.E32=0;m.E33=1;m.E41=2;m.E42=4;m.E43=8;}
struct C3DAnimationStats {
 void GetDeltaMovement(CMatrix3x4& m,float last,float current)const{check(last==state->expectedLast&&current==state->expectedCurrent);matrix(m);++state->delta;}
 bool HasRotation()const{return state->rotate;}
};
struct C3DAnimationInfo{C3DAnimationStats stats;CCountedPointer<const C3DAnimationStats> GetAnimationStats()const{return CCountedPointer<const C3DAnimationStats>(&stats);}};
struct CMeshDataBank{C3DAnimationInfo info;const C3DAnimationInfo* PeekAnimation(long n)const{check(n==17);++state->bank;return &info;}};
struct World{CMeshDataBank bank;const CMeshDataBank* PeekMeshBank()const{return &bank;}};
struct Appear{CMatrix3x4 GetPositionMatrix(float time)const{check(time==1);CMatrix3x4 m;matrix(m);return m;}};
struct CTCPhysicsBase{
 CRightHandedSet rh;CTCPhysicsBase():rh(C3DVector(0,1,0),C3DVector(1,0,0)){}
 const CRightHandedSet& GetRHSet()const{++state->rhs;return rh;}
 void SetRHSet(const CRightHandedSet& r){++state->set;check(state->rotate?same(r.GetUp(),-1,0,0)&&same(r.GetForward(),0,1,0):same(r.GetUp(),0,1,0)&&same(r.GetForward(),1,0,0));}
 float GetFacingAngleXY()const{++state->angle;return 0;}
 C3DVector GetFacingVector()const{++state->face;return C3DVector(1,0,0);}
};
struct CTCFlourishTarget{void ProcessActionMovementDelta(C3DVector& m)const{++state->changes;m.Z+=2;}};
struct Creature{
 CTCFlourishTarget flourish;CTCPhysicsBase physics;Appear appearance;
 float GetAnimDuration(long n)const{check(n==17);++state->duration;return 2;}
 CTCPhysicsBase* GetPhysicsTC(){return &physics;}Appear* GetTCGraphicAppearance(){return &appearance;}
 bool PeekTC(const CTCFlourishTarget** p)const{++state->peek;if(state->flourish)*p=&flourish;return state->flourish;}
 C3DVector GetPos()const{return C3DVector(16,32,64);}
};
struct CCombatActionBase{
 World world;Creature* PCreature;CAnimationEntry entry;CCountedPointer<CAnimationEntry> pentry;
 float RunningTimeSeconds,LastRunningTimeSeconds;long TotalTime;C3DVector PreferredMovementVector;bool PreferredFinalPosSet;
 CCombatActionBase(Creature& c):PCreature(&c),pentry(&entry),RunningTimeSeconds(1),LastRunningTimeSeconds(0),TotalTime(4),PreferredMovementVector(4,8,12),PreferredFinalPosSet(false){}
 const World& PeekWorld()const{return world;}long GetAnimationIndex()const{return 17;}
 long GetAnimDelayTime()const{++state->delays;return 2;}
 const CCountedPointer<CAnimationEntry>& GetPAnimationEntry()const{++state->entry;return pentry;}
 const CMeshDataBank* PeekMeshBank()const;float GetTimeThroughCurrentAnimation()const;float GetLastTimeThroughCurrentAnimation()const;
 C3DVector GetNextPos()const;
};
inline const CMeshDataBank* CCombatActionBase::PeekMeshBank() const { return PeekWorld().PeekMeshBank(); }
inline float CCombatActionBase::GetTimeThroughCurrentAnimation() const {
    float time = RunningTimeSeconds - (float)GetAnimDelayTime() / CMainGameComponent::GetConstantFPS();
    if (time < 0.0f) time = 0.0f;
    return time;
}
inline float CCombatActionBase::GetLastTimeThroughCurrentAnimation() const {
    float time = LastRunningTimeSeconds - (float)GetAnimDelayTime() / CMainGameComponent::GetConstantFPS();
    if (time < 0.0f) time = 0.0f;
    return time;
}
C3DVector CCombatActionBase::GetNextPos() const {
    const long fps=CMainGameComponent::GetConstantFPS();
    C3DVector movement_vector;
    long anim_index=GetAnimationIndex();
    if (PreferredFinalPosSet) {
        long world_tick_anim_duration=(long)(PCreature->GetAnimDuration(anim_index)*fps);
        movement_vector=PreferredMovementVector/(float)TotalTime;
    } else {
        const CAnimationEntry* panim=GetPAnimationEntry().Get();
        const C3DAnimationInfo* info=PeekMeshBank()->PeekAnimation(anim_index);
        CCountedPointer<const C3DAnimationStats> stats=info->GetAnimationStats();
        float current_time=GetTimeThroughCurrentAnimation();
        float last_time=GetLastTimeThroughCurrentAnimation();
        const C3DAnimationStats* anim_stats=stats.Get();
        CMatrix3x4 delta;
        anim_stats->GetDeltaMovement(delta,last_time,current_time);
        C3DVector delta_position_movement=delta.GetTranslation();
        PCreature->GetTCGraphicAppearance()->GetPositionMatrix(1.0f).TransformVector(delta_position_movement);
        CTCPhysicsBase* ptc_physics=PCreature->GetPhysicsTC();
        const CRightHandedSet& rhs=ptc_physics->GetRHSet();
        C3DVector forward=rhs.GetForward();
        C3DVector up=rhs.GetUp();
        if (anim_stats->HasRotation()) {
            delta.TransformVector(forward);
            delta.TransformVector(up);
            // The debug build checks each transformed component for NaN.
            FABLE_RETAIL_ASSERT(forward.X == forward.X);
            FABLE_RETAIL_ASSERT(forward.Y == forward.Y);
            FABLE_RETAIL_ASSERT(forward.Z == forward.Z);
            FABLE_RETAIL_ASSERT(up.X == up.X);
            FABLE_RETAIL_ASSERT(up.Y == up.Y);
            FABLE_RETAIL_ASSERT(up.Z == up.Z);
        }
        ptc_physics->SetRHSet(CRightHandedSet(up,forward));
        movement_vector=delta_position_movement;
        float xy=PCreature->GetPhysicsTC()->GetFacingAngleXY();
        C3DVector facing=PCreature->GetPhysicsTC()->GetFacingVector();
    }
    const CTCFlourishTarget* ptc_flourish_target;
    if (PCreature->PeekTC(&ptc_flourish_target)) ptc_flourish_target->ProcessActionMovementDelta(movement_vector);
    C3DVector next_pos=PCreature->GetPos()+movement_vector;
    return next_pos;
}

void cases(){
 for(int pref=0;pref<2;++pref)for(int rot=0;rot<2;++rot)for(int flour=0;flour<2;++flour)
 for(int earlier=0;earlier<3;++earlier)for(int current=0;current<3;++current){
  Case s;state=&s;Creature c;CCombatActionBase a(c);s.rotate=rot!=0;s.flourish=flour!=0;a.PreferredFinalPosSet=pref!=0;
  a.LastRunningTimeSeconds=earlier*0.5f;a.RunningTimeSeconds=current*0.5f;s.expectedLast=earlier==2?0.5f:0.0f;s.expectedCurrent=current==2?0.5f:0.0f;
  C3DVector next=a.GetNextPos();
  check(pref?same(next,17,34,67+2*flour):same(next,12,34,72+2*flour));
  check(s.duration==pref&&s.entry==1-pref&&s.bank==1-pref&&s.delta==1-pref&&s.set==1-pref&&s.rhs==1-pref);
  check(s.angle==1-pref&&s.face==1-pref&&s.peek==1&&s.changes==flour&&s.delays==2*(1-pref));
 }
}
}
int main(){MovementTest::cases();puts("008575c0_TEST PASS");return 0;}