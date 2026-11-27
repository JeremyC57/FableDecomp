
#include <stdio.h>
#include <math.h>
#include <deque>
#include <stdlib.h>
#define FABLE_RETAIL_ASSERT_ARM(x) abort()
enum EAIActionReturn { AI_ACTION_TERMINATE_PROCESS,AI_ACTION_FAILED,AI_ACTION_FINISHED,AI_ACTION_IN_PROGRESS };
enum { ACTION_REPEAT_CALLBACK, AI_MOVEMENT_FLYING };
const unsigned long NAVIGATOR_TYPE_FLYER=11,NAVIGATOR_TYPE_A_STAR=22;
const float DELTA_NOTIONAL_ZERO=0.001f;
float GFSqr(float x){return x*x;}
struct C3DVector{
 float X,Y,Z;
 C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}
 C3DVector operator+(const C3DVector& p)const{return C3DVector(X+p.X,Y+p.Y,Z+p.Z);}
 C3DVector operator-(const C3DVector& p)const{return C3DVector(X-p.X,Y-p.Y,Z-p.Z);}
 float GetSquaredMagnitude2D()const{return X*X+Y*Y;}
};
int stop,ncurves,failpath,nearpos,outcome,zone,moves,points,pops,navs,face,peeks;
struct CBezierCurve{C3DVector GetPointOnCurve(float t){++points;return C3DVector(t*10,0,0);}};
struct CTCCreatureNavigation{void SetNavigatorToType(unsigned long v){if(v!=(navs?NAVIGATOR_TYPE_A_STAR:NAVIGATOR_TYPE_FLYER))abort();++navs;}};
struct Physics{C3DVector GetFacingVector(){return C3DVector();} float GetRadius(){return 0;} void SetFacingAngleYZ(float v){if(v!=0)abort();++face;}};
struct Creature{CTCCreatureNavigation nav;Physics physics;bool GetTC(CTCCreatureNavigation** p){*p=&nav;return true;} Physics* GetPhysicsTC(){return &physics;} C3DVector GetPos(){return C3DVector();} float GetMaxFlyingSpeed(C3DVector){return nearpos?100:0;} C3DVector PeekPos(){++peeks;return C3DVector();}};
struct CBrainUpdateZone{long UpdateFrequency;};struct Brain{CBrainUpdateZone z;const CBrainUpdateZone* PeekPCurrentUpdateZone(){z.UpdateFrequency=2;return zone?&z:0;}};
struct CAIStateGroup_FlyingThingBase{Brain* PBrain; Creature creature;Creature& GetCreature(){return creature;} bool IsStopping(){return stop!=0;} bool CalculatePrettyPath(std::deque<CBezierCurve>& path,const C3DVector&){for(int i=0;i<ncurves;++i)path.push_back(CBezierCurve());return !failpath;}
 EAIActionReturn AIAction_MoveToPosProximity(int cb,C3DVector p,float d,bool& calculated,int mode,bool a,bool b,bool c,int e,bool f,float h){
 if(cb!=ACTION_REPEAT_CALLBACK||p.X<0||d!=0.01f||mode!=AI_MOVEMENT_FLYING||!a||b||!c||e||!f||h!=0)abort();
 ++moves;if(outcome==2)return AI_ACTION_FAILED;if(outcome==3)return AI_ACTION_TERMINATE_PROCESS;
 if(outcome==1&&!calculated){calculated=true;return AI_ACTION_IN_PROGRESS;}return AI_ACTION_FINISHED;}
 EAIActionReturn AIAction_RandomFlyToPos(const C3DVector&);};
EAIActionReturn CAIStateGroup_FlyingThingBase::AIAction_RandomFlyToPos(const C3DVector& pos)
{
    if (IsStopping())
        return AI_ACTION_TERMINATE_PROCESS;

    CTCCreatureNavigation* pnav;
    if (!GetCreature().GetTC(&pnav)) {
        FABLE_RETAIL_ASSERT_ARM(false);
    }
    pnav->SetNavigatorToType(NAVIGATOR_TYPE_FLYER);
    std::deque<CBezierCurve> pretty_path;
    if (!CalculatePrettyPath(pretty_path, pos)) {
        pnav->SetNavigatorToType(NAVIGATOR_TYPE_A_STAR);
        return AI_ACTION_FAILED;
    }

    const CBrainUpdateZone* pupdate_zone = PBrain->PeekPCurrentUpdateZone();
    long muliply_speed_factor = 1;
    if (pupdate_zone)
        muliply_speed_factor = pupdate_zone->UpdateFrequency;
    C3DVector to_pos = GetCreature().GetPos() + GetCreature().GetPhysicsTC()->GetFacingVector();
    float nav_close_enough_sq = GFSqr(0.01f + GetCreature().GetPhysicsTC()->GetRadius()
        + GetCreature().GetMaxFlyingSpeed(to_pos) * muliply_speed_factor);
    while (!pretty_path.empty()) {
        if (false) {} else for (float parameter_t = 0.1f; parameter_t <= 1.0f + DELTA_NOTIONAL_ZERO; parameter_t += 0.1f) {
            bool final_point = pretty_path.size() == 1 && fabs(parameter_t - 1.0f) < DELTA_NOTIONAL_ZERO;
            C3DVector target_pos = pretty_path.front().GetPointOnCurve(parameter_t);
            if (final_point || (target_pos - GetCreature().PeekPos()).GetSquaredMagnitude2D() >= nav_close_enough_sq) {
                EAIActionReturn ret = AI_ACTION_FAILED;
                bool close_enough = false;
                bool nav_route_calculated = false;
                do {
                    ret = AIAction_MoveToPosProximity(ACTION_REPEAT_CALLBACK, target_pos, 0.01f, nav_route_calculated,
                        AI_MOVEMENT_FLYING, true, false, true, 0, true, 0.0f);
                    close_enough = !final_point && (target_pos - GetCreature().PeekPos()).GetSquaredMagnitude2D() <= nav_close_enough_sq;
                } while (ret == AI_ACTION_IN_PROGRESS && !close_enough);
                if (ret != AI_ACTION_FINISHED && ret != AI_ACTION_IN_PROGRESS)
                    return ret;
            }
        }
        pretty_path.pop_front();
    }
    GetCreature().GetPhysicsTC()->SetFacingAngleYZ(0.0f);
    pnav->SetNavigatorToType(NAVIGATOR_TYPE_A_STAR);
    return AI_ACTION_FINISHED;
}

int main(){
 Brain brain; CAIStateGroup_FlyingThingBase g;g.PBrain=&brain;
 for(stop=0;stop<2;++stop)for(ncurves=0;ncurves<4;++ncurves)for(failpath=0;failpath<2;++failpath)
 for(nearpos=0;nearpos<2;++nearpos)for(outcome=0;outcome<4;++outcome)for(zone=0;zone<2;++zone){
 points=moves=navs=face=peeks=0;int expected=AI_ACTION_FINISHED,n=2,f=1,p=ncurves*10,m=nearpos?(ncurves!=0):p;
 if(stop){expected=AI_ACTION_TERMINATE_PROCESS;n=f=p=m=0;}
 else if(failpath){expected=AI_ACTION_FAILED;f=p=m=0;}
 else if(m&&outcome>=2){expected=outcome==2?AI_ACTION_FAILED:AI_ACTION_TERMINATE_PROCESS;n=1;f=0;m=1;if(!nearpos)p=1;}
 else if(outcome==1)m*=2;
 if(g.AIAction_RandomFlyToPos(C3DVector())!=expected||points!=p||moves!=m||navs!=n||face!=f)return 1;
 }
 puts("009130d0_TEST PASS");return 0;}