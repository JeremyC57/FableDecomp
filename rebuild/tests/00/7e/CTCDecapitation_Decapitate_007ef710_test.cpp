#include <stdio.h>
#include <math.h>
#include <string.h>
struct C3DVector {
 float X,Y,Z;
 C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}
 C3DVector operator+(const C3DVector& v)const{return C3DVector(X+v.X,Y+v.Y,Z+v.Z);}
 C3DVector operator*(float k)const{return C3DVector(X*k,Y*k,Z*k);}
 void Normalise(){*this=*this*(1.0f/(float)sqrt(X*X+Y*Y+Z*Z));}
 void Rotate(float x,float z){if(x||z)throw 1;}
};
struct CRightHandedSet {int id;};
typedef const char* CCharString;
static CCharString DECAPITATION_DUMMY_OBJECT_NAME="head";
static C3DVector MISSING_DUMMY_OBJECT_BODGE_VECTOR(0,0,1.6f);
static const float INITIAL_HEAD_ACCELERATION_MAGNITUDE=0.4f;
struct CTCHeroStats {long count;long GetNumberOfDecapitations(){return count;}void SetNumberOfDecapitations(long n){count=n;}};
struct CTCHeroOnlineScoreboard {enum {OSA_DECAPITATIONS=1};float score;void UpdateScoreForActivity(int a,float n,bool b){if(a!=1||b)throw 2;score=n;}};
struct CTCRandomAppearanceMorph {int calls;void UpdateDecapitationStatus(){++calls;}};
struct CThing;
struct CTCSolidToThing {CThing* ignored;void AddThingNotSolidTo(CThing* p){ignored=p;}};
struct CTCRotate {bool active;int calls;void SetRotating(bool v){active=v;}void SetRotationInfo(C3DVector v,float a,float b){if(v.X||v.Y||v.Z!=1||a!=0.05f||b!=2.2f)throw 3;++calls;}};
struct Physics {C3DVector acceleration;CRightHandedSet orientation;C3DVector GetFacingVector(){return C3DVector(0,1,0);}CRightHandedSet GetRHSet(){return orientation;}void ApplyAcceleration(C3DVector a){acceleration=a;}};
struct Appearance {bool found,force;int calls;bool GetDummyObject(CCharString s,int a,int b,C3DVector& p,CRightHandedSet& r,float f){if(strcmp(s,"head")||a||b||f!=1)throw 4;++calls;if(found){p=C3DVector(7,8,9);r.id=73;}return found;}void SetForceResendEveryRenderFrame(bool f){force=f;}};
struct CTCDParticleEmitter {int attached,creates;float height;CThing* parent;C3DVector pos;static CThing* Create(long,C3DVector,bool);void AttachToThing(CThing& t,int k,CCharString n,int z,float h){if(z||strcmp(n,k==10?"head":""))throw 5;attached=k;parent=&t;height=h;}};
struct CThing {bool alive,statsPresent,morphPresent;unsigned flags;CTCHeroStats stats;CTCHeroOnlineScoreboard board;CTCRandomAppearanceMorph morph;Appearance appearance;Physics physics;CTCDParticleEmitter emitter;
 CThing():alive(true),statsPresent(false),morphPresent(false),flags(0){stats.count=7;board.score=-1;morph.calls=0;appearance.found=appearance.force=false;appearance.calls=0;physics.orientation.id=19;emitter.creates=emitter.attached=0;emitter.parent=0;emitter.height=-1;}
 unsigned GetTypeFlags(){return flags;}bool GetTC(CTCHeroStats** p){*p=statsPresent?&stats:0;return statsPresent;}bool GetTC(CTCHeroOnlineScoreboard** p){*p=&board;return true;}bool GetTC(CTCRandomAppearanceMorph** p){*p=&morph;return morphPresent;}template<class T>bool HasTC(){return morphPresent;}Appearance* GetTCGraphicAppearance(){return &appearance;}Physics* GetPhysicsTC(){return &physics;}C3DVector GetPos(){return C3DVector(1,2,3);}long GetPlayerNumber(){return 6;}void GetTCDriver(CTCDParticleEmitter** p){*p=&emitter;}
};
struct CThingCreatureBase: CThing {CThing* attacker;CThing* GetPLastAttackedByCreature()const{return attacker;}};
struct CThingObject: CThing {int creates;C3DVector pos;int orientation;CTCSolidToThing solid;CTCRotate rotate;bool canRotate;static CThingObject* Create(long,C3DVector,long,const CRightHandedSet*,unsigned,CCharString);void* AddTC(CCharString n,bool flag,int spare){if(spare)throw 6;if(!strcmp(n,"CTCSolidToThing")&&flag)return &solid;if(!strcmp(n,"CTCRotate")&&!flag)return canRotate?&rotate:0;throw 7;}};
static CThingObject* head_result;static CThing* particle_result;
CThingObject* CThingObject::Create(long id,C3DVector p,long player,const CRightHandedSet* set,unsigned k,CCharString name){if(id!=2||player!=6||k||*name)throw 8;++head_result->creates;head_result->pos=p;head_result->orientation=set->id;return head_result;}
CThing* CTCDParticleEmitter::Create(long id,C3DVector p,bool b){if(id!=3||b)throw 9;++particle_result->emitter.creates;particle_result->emitter.pos=p;return particle_result;}
template<class T>bool IS_THING_ALIVE(T p){return p&&p->alive;}
struct Player {CThing* controlled;CThing* GetPControlledCreature(){return controlled;}Player& GetMainPlayer(){return *this;}};
struct Def {long Head,BodyParticleEffect;};
struct CTCDecapitation {bool Decapitated;CThingCreatureBase parent;Def def;Def* PDef;Player player;Player& GetPlayerManager(){return player;}CThing& GetParentThing(){return parent;}void Decapitate();};
void CTCDecapitation::Decapitate()
{
    if (Decapitated)
        return;
    if (GetParentThing().GetTypeFlags() & 8) {
        const CThingCreatureBase& parent_creature=static_cast<const CThingCreatureBase&>(GetParentThing());
        CThing* plast_attacker=parent_creature.GetPLastAttackedByCreature();
        CTCHeroStats* ptc_hero_stats;
        if (IS_THING_ALIVE(plast_attacker) && plast_attacker->GetTC(&ptc_hero_stats)) {
            ptc_hero_stats->SetNumberOfDecapitations(ptc_hero_stats->GetNumberOfDecapitations()+1);
            CTCHeroOnlineScoreboard* ptc_hero_scoreboard;
            plast_attacker->GetTC(&ptc_hero_scoreboard);
            ptc_hero_scoreboard->UpdateScoreForActivity(CTCHeroOnlineScoreboard::OSA_DECAPITATIONS,(float)ptc_hero_stats->GetNumberOfDecapitations(),false);
        }
    }
    if(!GetParentThing().HasTC<CTCRandomAppearanceMorph>())return;
    Decapitated=true;
    CTCRandomAppearanceMorph* ptc_random_appearance_morph;
    GetParentThing().GetTC(&ptc_random_appearance_morph);
    ptc_random_appearance_morph->UpdateDecapitationStatus();
    C3DVector headPos;
    CRightHandedSet headSet;
    bool found = GetParentThing().GetTCGraphicAppearance()->GetDummyObject(DECAPITATION_DUMMY_OBJECT_NAME, 0, 0, headPos, headSet, 1.0f);
    if (!found) {
        headPos=GetParentThing().GetPos()+MISSING_DUMMY_OBJECT_BODGE_VECTOR;
        headSet = GetParentThing().GetPhysicsTC()->GetRHSet();
    }
    if (PDef->Head) {
        CThingObject* head = CThingObject::Create(PDef->Head, headPos, GetParentThing().GetPlayerNumber(), &headSet, 0, "");
        if (IS_THING_ALIVE(head)) {
            CTCSolidToThing* solid = (CTCSolidToThing*)head->AddTC("CTCSolidToThing", true, 0);
            solid->AddThingNotSolidTo(GetPlayerManager().GetMainPlayer().GetPControlledCreature());
            C3DVector initial_head_acceleration=GetParentThing().GetPhysicsTC()->GetFacingVector()+C3DVector(0.0f,0.0f,0.3f);
            initial_head_acceleration.Normalise();
            head->GetPhysicsTC()->ApplyAcceleration(initial_head_acceleration*INITIAL_HEAD_ACCELERATION_MAGNITUDE);
            CTCRotate* rotate = (CTCRotate*)head->AddTC("CTCRotate", false, 0);
            if (rotate) {
                C3DVector axis(0.0f, 0.0f, 1.0f);
                axis.Rotate(0.0f, 0.0f);
                rotate->SetRotationInfo(axis, 0.05f, 2.2f);
                rotate->SetRotating(true);
            }
            head->GetTCGraphicAppearance()->SetForceResendEveryRenderFrame(true);
        }
    }
    if (PDef->BodyParticleEffect > 0) {
        CThing* blood = CTCDParticleEmitter::Create(PDef->BodyParticleEffect, headPos, false);
        if (IS_THING_ALIVE(blood)) {
            CTCDParticleEmitter* emitter;
            blood->GetTCDriver(&emitter);
            if (found)
                emitter->AttachToThing(GetParentThing(), 0xa, DECAPITATION_DUMMY_OBJECT_NAME, 0, 0.0f);
            else
                emitter->AttachToThing(GetParentThing(), 0xa8, "", 0, MISSING_DUMMY_OBJECT_BODGE_VECTOR.Z);
        }
    }
}

static bool near(float a,float b){return fabs(a-b)<0.00001;}
int main(){
 for(int f=0;f<2048;++f)for(int particle=-1;particle<=1;++particle){
  CTCDecapitation w;CThing attacker,player,blood;CThingObject head;
  head_result=&head;particle_result=&blood;head.creates=0;head.rotate.active=false;head.rotate.calls=0;head.solid.ignored=0;
  bool already=(f&1)!=0,creature=(f&2)!=0,known=(f&4)!=0,alive=(f&8)!=0,stats=(f&16)!=0,morph=(f&32)!=0,dummy=(f&64)!=0,hasHead=(f&128)!=0,headAlive=(f&256)!=0,canRotate=(f&512)!=0,bloodAlive=(f&1024)!=0;
  w.Decapitated=already;w.PDef=&w.def;w.def.Head=hasHead?2:0;w.def.BodyParticleEffect=particle>0?3:particle;
  w.parent.flags=creature?8:0;w.parent.attacker=known?&attacker:0;attacker.alive=alive;attacker.statsPresent=stats;w.parent.morphPresent=morph;w.parent.appearance.found=dummy;head.alive=headAlive;head.canRotate=canRotate;blood.alive=bloodAlive;w.player.controlled=&player;
  w.Decapitate();
  bool score=!already&&creature&&known&&alive&&stats,doMorph=!already&&morph,doHead=doMorph&&hasHead,attach=doMorph&&particle>0&&bloodAlive;
  if(attacker.stats.count!=7+score||attacker.board.score!=(score?8:-1)||w.Decapitated!=(already||morph)||w.parent.morph.calls!=doMorph||w.parent.appearance.calls!=doMorph)return 1;
  if(head.creates!=doHead||head.appearance.force!=(doHead&&headAlive)||head.solid.ignored!=(doHead&&headAlive?&player:0)||head.rotate.calls!=(doHead&&headAlive&&canRotate)||head.rotate.active!=(head.rotate.calls!=0))return 2;
  float z=dummy?9:4.6f;
  if(doHead&&(head.pos.X!=(dummy?7:1)||head.pos.Y!=(dummy?8:2)||!near(head.pos.Z,z)||head.orientation!=(dummy?73:19)))return 3;
  if(doHead&&headAlive&&(!near(head.physics.acceleration.Y,0.4f/(float)sqrt(1.09))||!near(head.physics.acceleration.Z,0.12f/(float)sqrt(1.09))))return 4;
  if(blood.emitter.creates!=(doMorph&&particle>0)||blood.emitter.attached!=(attach?(dummy?10:168):0)||blood.emitter.parent!=(attach?&w.parent:0))return 5;
  if(attach&&(!near(blood.emitter.height,dummy?0:1.6f)||!near(blood.emitter.pos.Z,z)))return 6;
 }
 puts("007ef710_TEST PASS");return 0;
}