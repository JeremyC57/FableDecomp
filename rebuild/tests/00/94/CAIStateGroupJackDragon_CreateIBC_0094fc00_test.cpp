#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
static void check(bool p,const char*s){if(!p){printf("FAIL %s\n",s);exit(1);}}
struct CCharString{const char*p;CCharString(const char*s="",long=-1):p(s){}};
struct C3DVector{float X,Y,Z;C3DVector(){}C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}C3DVector(const C3DVector&p){X=p.X;Y=p.Y;Z=p.Z;}const C3DVector operator+(const C3DVector&o)const{return C3DVector(X+o.X,Y+o.Y,Z+o.Z);}const C3DVector operator*(float s)const{return C3DVector(X*s,Y*s,Z*s);}};
struct CRightHandedSet{C3DVector Up,Forward;};
struct GraphicDouble{C3DVector mouth;bool valid;int calls;GraphicDouble():mouth(4,5,99),valid(true),calls(0){}bool GetDummyObject(const CCharString&name,long index,unsigned long flags,C3DVector&pos,CRightHandedSet&,float scale){check(strcmp(name.p,"DRAGON_MOUTH_DUMMY")==0&&index==0&&flags==0&&scale==1,"mouth dummy arguments");++calls;if(valid)pos=mouth;return valid;}};
struct PhysicsDouble{C3DVector facing;PhysicsDouble():facing(0.25f,-0.5f,0.75f){}C3DVector GetFacingVector()const{return facing;}};
struct CThing{bool dead;CThing():dead(false){}bool IsDead()const{return dead;}};
struct CThingAICreatureInit{void*OriginalThing;void*PreviousThing;CCharString ScriptName,OverridingBrainName;bool PlayerCharacter,Drawable,PlaceAtEndOfList,EnableCreatureAutoPlacing;void*HomeBuilding;void*WorkBuilding;CThingAICreatureInit(void*original=0,void*previous=0,bool pc=false,const CCharString&script="",const CCharString&brain="",bool drawable=true):OriginalThing(original),PreviousThing(previous),ScriptName(script),OverridingBrainName(brain),PlayerCharacter(pc),Drawable(drawable),PlaceAtEndOfList(true),EnableCreatureAutoPlacing(false),HomeBuilding(0),WorkBuilding(0){}};
enum{TCI_SOUND_PLAYER=16};
struct CThingAICreature:CThing{C3DVector pos;GraphicDouble graphic;PhysicsDouble physics;unsigned long player;int removals,hides;bool hidden;static CThingAICreature*created;static C3DVector createPos;static long createIndex,createPlayer;static int creates;CThingAICreature():pos(1,2,3),player(2),removals(0),hides(0),hidden(false){}const C3DVector&GetPos()const{return pos;}GraphicDouble*GetTCGraphicAppearance(){return &graphic;}PhysicsDouble*GetPhysicsTC(){return &physics;}unsigned long GetPlayerNumber()const{return player;}void RemoveTC(int type){check(type==TCI_SOUND_PLAYER,"remove sound TC");++removals;}void SetHiddenOnMiniMap(bool b){++hides;hidden=b;}static CThingAICreature*Create(long index,const C3DVector&p,long player,const CThingAICreatureInit&init){++creates;createPos=p;createIndex=index;createPlayer=player;check(!init.OriginalThing&&!init.PreviousThing&&!init.PlayerCharacter&&init.Drawable&&init.PlaceAtEndOfList&&!init.EnableCreatureAutoPlacing&&!init.HomeBuilding&&!init.WorkBuilding&&strcmp(init.ScriptName.p,"")==0&&strcmp(init.OverridingBrainName.p,"")==0,"original default init contract");return created;}};CThingAICreature*CThingAICreature::created=0;C3DVector CThingAICreature::createPos;long CThingAICreature::createIndex=0,CThingAICreature::createPlayer=0;int CThingAICreature::creates=0;
template<class T>struct CIntelligentPointer{T*p;CIntelligentPointer(T*q=0):p(q){}};
template<class T>bool IS_THING_ALIVE(const T&t){if(t!=0){if(!t->IsDead())return true;}return false;}
struct CGameDefinitionManager{static CGameDefinitionManager&Get(){static CGameDefinitionManager x;return x;}long GetDefGlobalIndexFromName(const CCharString&name)const{check(strcmp(name.p,"CREATURE_INVISIBLE_BODGE")==0,"bodge definition name");return 77;}};
struct CAIStateGroup_JackDragon{CThingAICreature dragon;CIntelligentPointer<CThingAICreature>PInvisibleBodgeCreature;CThingAICreature&GetCreature(){return dragon;}void CreateIBC();};

void CAIStateGroup_JackDragon::CreateIBC()
{
 long bodge_creature_index=CGameDefinitionManager::Get().GetDefGlobalIndexFromName("CREATURE_INVISIBLE_BODGE");
 C3DVector mouth_pos=GetCreature().GetPos();
 CRightHandedSet rhs;
 GetCreature().GetTCGraphicAppearance()->GetDummyObject("DRAGON_MOUTH_DUMMY",0,0,mouth_pos,rhs,1.0f);
 mouth_pos.Z=GetCreature().GetPos().Z;
 C3DVector create_pos=mouth_pos+GetCreature().GetPhysicsTC()->GetFacingVector()*1.0f;
 CThingAICreature* p_ibc=CThingAICreature::Create(bodge_creature_index,create_pos,GetCreature().GetPlayerNumber(),CThingAICreatureInit());
 if(IS_THING_ALIVE(p_ibc)) {
  p_ibc->RemoveTC(TCI_SOUND_PLAYER);
  p_ibc->SetHiddenOnMiniMap(true);
  PInvisibleBodgeCreature=CIntelligentPointer<CThingAICreature>(p_ibc);
 }
}
static bool eq(const C3DVector&a,const C3DVector&b){return fabs(a.X-b.X)<0.00003&&fabs(a.Y-b.Y)<0.00003&&fabs(a.Z-b.Z)<0.00003;}
int main(){
 {CAIStateGroup_JackDragon g;CThingAICreature ibc;CThingAICreature::created=&ibc;g.CreateIBC();check(CThingAICreature::createIndex==77&&CThingAICreature::createPlayer==2&&eq(CThingAICreature::createPos,C3DVector(4.25f,4.5f,3.75f)),"spawn from dummy XY, restored height, unit facing offset");check(g.PInvisibleBodgeCreature.p==&ibc&&ibc.removals==1&&ibc.hides==1&&ibc.hidden,"live bodge configured and retained");check(g.dragon.graphic.calls==1,"one mouth lookup");}
 {CAIStateGroup_JackDragon g;CThingAICreature old,dead;dead.dead=true;g.PInvisibleBodgeCreature=CIntelligentPointer<CThingAICreature>(&old);CThingAICreature::created=&dead;g.CreateIBC();check(g.PInvisibleBodgeCreature.p==&old&&dead.removals==0&&dead.hides==0,"dead creation preserves prior pointer and skips effects");}
 {CAIStateGroup_JackDragon g;CThingAICreature old;g.PInvisibleBodgeCreature=CIntelligentPointer<CThingAICreature>(&old);CThingAICreature::created=0;g.CreateIBC();check(g.PInvisibleBodgeCreature.p==&old,"null creation preserves pointer");}
 {CAIStateGroup_JackDragon g;CThingAICreature ibc;CThingAICreature::created=&ibc;g.dragon.graphic.valid=false;g.dragon.player=7;g.dragon.physics.facing=C3DVector(-2,1,-1);g.CreateIBC();check(eq(CThingAICreature::createPos,C3DVector(-1,3,2))&&CThingAICreature::createPlayer==7,"release dummy failure retains creature position and forwards player");}
 check(CThingAICreature::creates==4,"one creation per invocation");puts("0094fc00_TEST PASS");return 0;
}