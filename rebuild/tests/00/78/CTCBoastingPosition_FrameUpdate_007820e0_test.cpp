#include <stdio.h>
#define FABLE_RETAIL_C3DVECTOR
#include "engine/C3DVector.h"
#include "engine/GFSqr.h"
enum {TCI_HERO=63};
struct CThingGameObject;
struct CThingCreatureBase;
static unsigned enters,leaves,selections,frames,renders,searches;
static bool affordable,bad,hasStats,hasTarget;
static CThingCreatureBase* selectedHero;
struct CTCHeroStats {long coins;long GetMoney()const{return coins;}};
static CTCHeroStats stats;
struct CTCBoastingPodium {
    void OnHeroEnterBoastArea(long i,bool value,CThingCreatureBase& who){++enters;if(i!=42 || &who!=selectedHero)bad=true;affordable=value;}
    void OnHeroLeaveBoastArea(long i,bool value,CThingCreatureBase& who){++leaves;if(i!=42 || &who!=selectedHero)bad=true;affordable=value;}
};
static CTCBoastingPodium podium;
struct CTCTargetingBase {void SetPTargetedThing(CThingGameObject*);};
static CTCTargetingBase targeting;
struct CThingGameObject {
    bool alive; C3DVector pos;
    const C3DVector& GetPos()const{return pos;}
    void GetTC(CTCBoastingPodium** out){*out=&podium;}
    void SetAsFrameUpdated(){++frames;}
    void SetAsRenderedThisFrame(unsigned reason){++renders;if(reason!=4)bad=true;}
};
static CThingGameObject parent,podiumObject;
void CTCTargetingBase::SetPTargetedThing(CThingGameObject* object){++selections;if(object!=&parent)bad=true;}
struct CThingCreatureBase: CThingGameObject {
    bool GetTC(CTCTargetingBase** out){*out=hasTarget?&targeting:0;return hasTarget;}
    bool PeekTC(const CTCHeroStats** out){*out=hasStats?&stats:0;return hasStats;}
};
static bool IS_THING_ALIVE(const CThingGameObject* p){return p && p->alive;}
static CThingGameObject* GFDebugDynamicCast(CThingGameObject* p,CThingGameObject*){return p;}
struct CThingFilter_HasTC {int type;explicit CThingFilter_HasTC(int t):type(t){}};
struct CThingCompare_Nearest {C3DVector pos;explicit CThingCompare_Nearest(const C3DVector& p):pos(p){}};
struct Search {
    bool GetPBestThing(const CThingFilter_HasTC& f,const CThingCompare_Nearest& c,CThingCreatureBase** out){
        ++searches;if(f.type!=TCI_HERO || c.pos.X!=parent.pos.X || c.pos.Y!=parent.pos.Y || c.pos.Z!=parent.pos.Z)bad=true;
        *out=selectedHero;return selectedHero!=0;
    }
};
static Search search;
static Search& GetThingSearchTools(){return search;}
struct CTCBoastingPosition {
    CThingGameObject* PBoastingPodium;C3DVector OrbPosition;
    bool PositionActivated;long BoastIndex,Cost;bool BoastOn,HeroInsideRadius;
    CThingGameObject& GetParentThing(){return parent;}
    void FrameUpdate();
};
void CTCBoastingPosition::FrameUpdate()
{
    if (IS_THING_ALIVE(PBoastingPodium) && PositionActivated) {
        CThingCreatureBase* phero;
        GetThingSearchTools().GetPBestThing(CThingFilter_HasTC(TCI_HERO),CThingCompare_Nearest(GetParentThing().GetPos()),&phero);
        if (IS_THING_ALIVE(phero) && BoastOn) {
            const CTCHeroStats* ptc_hero_stats;
            bool hero_can_afford_boast=phero->PeekTC(&ptc_hero_stats) ? ptc_hero_stats->GetMoney()>=Cost : true;
            if ((phero->GetPos()-OrbPosition).GetSquaredMagnitude()<GFSqr(1.5f) &&
                (phero->GetPos()-PBoastingPodium->GetPos()).GetSquaredMagnitude2D()<
                (GetParentThing().GetPos()-PBoastingPodium->GetPos()).GetSquaredMagnitude2D()) {
                if (!HeroInsideRadius) {
                    CTCBoastingPodium* ptc_boasting_podium;
                    PBoastingPodium->GetTC(&ptc_boasting_podium);
                    ptc_boasting_podium->OnHeroEnterBoastArea(BoastIndex,hero_can_afford_boast,*phero);
                    HeroInsideRadius=true;
                }
                CTCTargetingBase* ptc_targeting;
                if (phero->GetTC(&ptc_targeting)) {
                    CThingGameObject& parent_game_object=*GFDebugDynamicCast(&GetParentThing(),(CThingGameObject*)0);
                    ptc_targeting->SetPTargetedThing(&parent_game_object);
                }
            } else if (HeroInsideRadius) {
                CTCBoastingPodium* ptc_boasting_podium;
                PBoastingPodium->GetTC(&ptc_boasting_podium);
                ptc_boasting_podium->OnHeroLeaveBoastArea(BoastIndex,hero_can_afford_boast,*phero);
                HeroInsideRadius=false;
            }
        }
    }
    GetParentThing().SetAsFrameUpdated();
    GetParentThing().SetAsRenderedThisFrame(4);
}

int main(){
 CThingCreatureBase hero;
 for(int pod=0;pod<3;++pod)for(int active=0;active<2;++active)
 for(int live=0;live<3;++live)for(int on=0;on<2;++on)
 for(int loc=0;loc<6;++loc)for(int inside=0;inside<2;++inside)
 for(int money=0;money<4;++money)for(int target=0;target<2;++target){
    CTCBoastingPosition pos;
    pos.BoastIndex=42;pos.Cost=10;pos.HeroInsideRadius=inside!=0;pos.BoastOn=on!=0;
    pos.PositionActivated=active!=0;pos.PBoastingPodium=pod?&podiumObject:0;
    podiumObject.alive=pod==2;selectedHero=live?&hero:0;hero.alive=live==2;
    hasStats=money!=0;stats.coins=money==1?9:money==2?10:11;hasTarget=target!=0;
    parent.pos.X=2;parent.pos.Y=0;parent.pos.Z=7;
    podiumObject.pos.X=podiumObject.pos.Y=podiumObject.pos.Z=0;
    hero.pos.X=loc==3?2.0f:loc==4?3.0f:1.0f;hero.pos.Y=0;hero.pos.Z=loc==5?18.0f:0.0f;
    pos.OrbPosition=hero.pos;pos.OrbPosition.Z+=loc==1?1.5f:loc==2?2.0f:0.0f;
    enters=leaves=selections=frames=renders=searches=0;affordable=bad=false;
    pos.FrameUpdate();
    bool find=pod==2 && active;bool evaluate=find && live==2 && on;
    bool within=loc==0 || loc==5;
    bool enter=evaluate && within && !inside;bool leave=evaluate && !within && inside;
    if(bad || searches!=unsigned(find) || frames!=1 || renders!=1 ||
       enters!=unsigned(enter) || leaves!=unsigned(leave) || selections!=unsigned(evaluate && within && target) ||
       ((enter||leave) && affordable!=(money!=1)) || pos.HeroInsideRadius!=(evaluate?within:inside!=0))return 1;
 }
 puts("007820e0_TEST PASS");return 0;
}