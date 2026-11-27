#include <cassert>
#include <cmath>
#include <cstdio>
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_ROTATE
#define FABLE_RETAIL_CRIGHTHANDEDSET_SET
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/ETCInterfaceType.h"
struct Scenario {
    int portal, findCalls, navFrom, enteredTry, searchCalls, losChecks, navChecks, resetCalls, pauses;
    bool villageAvailable, livingPlayer, playerAvailable, navigation, occupied, taskAvailable, startAvailable, accepted, autoplaced;
    long fine; float rotationAngle; C3DVector candidate, chosen;
};
static Scenario state;
void C3DVector::RotateXY(float angle) {
    assert(X==1 && Y==0 && Z==0);
    const double turn=angle*6.283185307179586;
    X=(float)cos(turn); Y=(float)sin(turn); state.rotationAngle=angle;
}
void CRightHandedSet::Rotate(const C3DVector& axis,float turns) {
    assert(&axis==&GetUp() && axis.Z==1 && GetForward().X==1 && turns==0.5f);
    SetForward(C3DVector(-1,0,0));
}
class CTCPhysicsBase {
public:
    CRightHandedSet basis; C3DVector pos;
    CTCPhysicsBase():pos(2,3,4) { basis.Set(C3DVector(0,0,1),C3DVector(1,0,0)); }
    const CRightHandedSet& GetRHSet() const { return basis; }
    const C3DVector& GetPosition() const { return pos; }
    void ResetPosition(const C3DVector& value) {
        assert(state.pauses==1 && value.X==state.candidate.X && value.Y==state.candidate.Y && value.Z==7.5f);
        state.chosen=value; ++state.resetCalls;
    }
    void ResetRHSet(const CRightHandedSet& value) { assert(state.resetCalls==1 && value.GetForward().X==-1 && value.GetUp().Z==1); ++state.resetCalls; }
};
class CTCCreatureNavigation {
public:
    bool IsPosNavigable(const C3DVector& value) const {
        ++state.losChecks; assert(value.Z==5.5f && fabs(value.X-2-8*cos(state.rotationAngle*6.283185307179586))<0.00001);
        assert(fabs(value.Y-3-8*sin(state.rotationAngle*6.283185307179586))<0.00001); state.candidate=value; return state.losChecks>=state.navFrom;
    }
    void InterruptNavigation() { if(state.pauses)assert(state.resetCalls==2); ++state.pauses; }
    float FindPosZAt(const C3DVector& value) const { assert(state.pauses==1 && value.Z==5.5f); return 7.5f; }
};
static CTCPhysicsBase playerPhysics,guardPhysics;
static CTCCreatureNavigation nav;
class CThing {
public:
    C3DVector position; CThing():position(12,4,8){}
    const C3DVector& GetPos() const { return position; }
};
class CThingPlayerCreature: public CThing {public: CTCPhysicsBase* GetPhysicsTC(){return &playerPhysics;}};
class CThingAICreature: public CThing {
public:
    CTCPhysicsBase* GetPhysicsTC(){return &guardPhysics;}
    bool GetTC(CTCCreatureNavigation** p){++state.navChecks;if(!state.navigation)return false;*p=&nav;return true;}
    void AutoPlaceNear(const C3DVector& position,float radius){assert(state.accepted && position.X==36 && radius==3.0f);state.autoplaced=true;}
};
static CThingAICreature guard;static CThing node;static CThingPlayerCreature hero;
bool IS_THING_ALIVE(const CThingPlayerCreature* p){return p && state.livingPlayer;}
class CPlayer {public:CThingPlayerCreature* GetPControlledCreature(){return state.playerAvailable?&hero:0;}};
class CPlayerManager {CPlayer player;public:CPlayer& GetMainPlayer(){return player;}};
static CPlayerManager players;
class CVillageTaskPatrol {public:CThing* GetPatrolStart()const{return state.startAvailable?&node:0;}};
static CVillageTaskPatrol task;
class CTCVillage {
public:
 long GetFineAmountForCurrentCrimes(){return state.fine;}
 CVillageTaskPatrol* GetFirstUnAssignedPatrolTask(){++state.findCalls;return state.taskAvailable?&task:0;}
 bool SetPatrolTaskAsAcceptedByThing(const CVillageTaskPatrol& t,const CThing& g){assert(&t==&task && &g==&guard);state.accepted=true;return true;}
};
static CTCVillage village;
class CTCVillageMember {public:CTCVillage* GetPVillage(){return state.villageAvailable?&village:0;}};
static CTCVillageMember member;
struct CThingCompare_Nearest {C3DVector pos;CThingCompare_Nearest(const C3DVector& p):pos(p){}};
struct CThingFilter_In2DRange {C3DVector pos;float range;CThingFilter_In2DRange(const C3DVector& p,float r):pos(p),range(r){}};
struct CThingFilter_HasTC {ETCInterfaceType type;CThingFilter_HasTC(ETCInterfaceType t):type(t){}};
struct Both {ETCInterfaceType type;CThingFilter_In2DRange area;Both(ETCInterfaceType t,const CThingFilter_In2DRange& a):type(t),area(a){}};
Both GFFilter_Bind2(const CThingFilter_HasTC& t,const CThingFilter_In2DRange& a){return Both(t.type,a);}
class CThingSearchTools {
public:
 bool GetPBestThing(const Both& filters,const CThingCompare_Nearest& comp,CThing** out) const {
    ++state.enteredTry;assert(filters.area.range==0.4f && comp.pos.X==hero.position.X && filters.area.pos.Z==hero.position.Z);
    assert(filters.type==(state.enteredTry==1?TCI_REGION_ENTRANCE:TCI_TELEPORTER));
    if(state.portal==state.enteredTry){*out=&hero;return true;}*out=0;return false;
 }
 bool GetPBestThing(const CThingFilter_In2DRange& area,const CThingCompare_Nearest& comp,CThing** out) const {
    ++state.searchCalls;assert(area.range==0.4f && comp.pos.X==state.candidate.X && area.pos.Y==state.candidate.Y);
    *out=state.occupied?&node:0;return *out!=0;
 }
};
static CThingSearchTools searchTools;
class CAIStateGroup_VillageTaskPatrol {
public:
 CVillageTaskPatrol* PTask; CTCVillageMember* PTCVillageMember;
 CPlayerManager& GetPlayerManager(){return players;} const CThingSearchTools& PeekThingSearchTools(){return searchTools;}
 CThingAICreature& GetCreature(){return guard;} bool InitCreaturePosition();
};
bool CAIStateGroup_VillageTaskPatrol::InitCreaturePosition()
{
    PTask = 0;
    CTCVillage* pvillage = PTCVillageMember->GetPVillage();
    if (pvillage) {
        CThingPlayerCreature* player = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        if (pvillage->GetFineAmountForCurrentCrimes() > 0 && IS_THING_ALIVE(player)) {
            CThing* thing;
            bool entered = false;
            if (PeekThingSearchTools().GetPBestThing(GFFilter_Bind2(CThingFilter_HasTC(TCI_REGION_ENTRANCE), CThingFilter_In2DRange(player->GetPos(), 0.4f)), CThingCompare_Nearest(player->GetPos()), &thing))
                entered = true;
            else if (PeekThingSearchTools().GetPBestThing(GFFilter_Bind2(CThingFilter_HasTC(TCI_TELEPORTER), CThingFilter_In2DRange(player->GetPos(), 0.4f)), CThingCompare_Nearest(player->GetPos()), &thing))
                entered = true;
            if (entered) {
                CTCPhysicsBase* tc_physics = player->GetPhysicsTC();
                if (false) {} else for (float angle = 0.0f; (float)fabs(angle) < 0.25; angle = angle > 0.0f ? -angle : -angle + 0.02f) {
                    C3DVector offset = tc_physics->GetRHSet().GetForward();
                    offset.RotateXY(angle);
                    C3DVector guard_pos = tc_physics->GetPosition() + offset * 8.0f;
                    guard_pos.Z += 1.5;
                    CRightHandedSet guard_rhset = tc_physics->GetRHSet();
                    guard_rhset.Rotate(guard_rhset.GetUp(), 0.5f);
                    CTCCreatureNavigation* tc_creature_navigation;
                    if (GetCreature().GetTC(&tc_creature_navigation) && tc_creature_navigation->IsPosNavigable(guard_pos)) {
                        if (!PeekThingSearchTools().GetPBestThing(CThingFilter_In2DRange(guard_pos, 0.4f), CThingCompare_Nearest(guard_pos), &thing)) {
                            CTCCreatureNavigation* nav;
                            GetCreature().GetTC(&nav);
                            nav->InterruptNavigation();
                            guard_pos.Z = tc_creature_navigation->FindPosZAt(guard_pos);
                            GetCreature().GetPhysicsTC()->ResetPosition(guard_pos);
                            GetCreature().GetPhysicsTC()->ResetRHSet(guard_rhset);
                            nav->InterruptNavigation();
                            return true;
                        }
                    }
                }
            }
        }
        PTask = pvillage->GetFirstUnAssignedPatrolTask();
        if (PTask != 0 && PTask->GetPatrolStart()) {
            pvillage->SetPatrolTaskAsAcceptedByThing(*PTask, GetCreature());
            GetCreature().AutoPlaceNear(PTask->GetPatrolStart()->GetPos(), 3.0f);
            return true;
        }
    }
    return false;
}

int main() {
    node.position.X=36;
    long cases=0;
    for(unsigned mask=0; mask<128; ++mask) for(int fine=-1;fine<=1;++fine)
    for(int portal=0;portal<=2;++portal) for(int threshold=0;threshold<4;++threshold) {
        
        state.findCalls=state.enteredTry=state.searchCalls=state.losChecks=state.navChecks=state.resetCalls=state.pauses=0;
        state.accepted=state.autoplaced=false; state.portal=portal;state.fine=fine;
        state.navFrom=threshold==3?100:1+threshold;state.villageAvailable=(mask&1)!=0;state.playerAvailable=(mask&2)!=0;
        state.livingPlayer=(mask&4)!=0;state.taskAvailable=(mask&8)!=0;state.startAvailable=(mask&16)!=0;
        state.navigation=(mask&32)!=0;state.occupied=(mask&64)!=0;
        CAIStateGroup_VillageTaskPatrol group;group.PTCVillageMember=&member;group.PTask=&task;
        bool guarded=state.villageAvailable && fine>0 && state.playerAvailable && state.livingPlayer;
        bool scan=guarded && portal!=0;
        bool placed=scan && state.navigation && !state.occupied && threshold!=3;
        bool fallback=state.villageAvailable && !placed;bool patrol=fallback && state.taskAvailable && state.startAvailable;
        assert(group.InitCreaturePosition()==(placed||patrol));
        assert(state.accepted==patrol && state.autoplaced==patrol && state.findCalls==(int)fallback);
        assert(group.PTask==(fallback && state.taskAvailable?&task:0));
        assert(state.enteredTry==(guarded?(portal==1?1:2):0));
        assert(state.resetCalls==(placed?2:0) && state.pauses==(placed?2:0));
        int tries=0;float angle=0;
        while(scan && fabs(angle)<0.25){++tries;if(placed && tries==state.navFrom)break;angle=angle>0?-angle:0.02f-angle;}
        assert(state.navChecks==tries+(placed?1:0));assert(state.losChecks==(state.navigation?tries:0));
        assert(state.searchCalls==(state.navigation && tries>=state.navFrom?tries-state.navFrom+1:0));++cases;
    }
    assert(cases==4608);puts("0093ef30_TEST PASS");return 0;
}