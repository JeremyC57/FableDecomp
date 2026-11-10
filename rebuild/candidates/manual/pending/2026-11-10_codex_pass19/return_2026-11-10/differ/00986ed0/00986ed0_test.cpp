#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <cstddef>
#include <new>
static void check(bool b){if(!b)std::abort();}
#pragma pack(push,1)
struct C3DVector{float X,Y,Z;C3DVector(){}C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}C3DVector operator-()const{return C3DVector(-X,-Y,-Z);}};
#pragma pack(pop)
struct CRightHandedSet{C3DVector Up,Forward;C3DVector GetRight()const{return C3DVector(Forward.Y*Up.Z-Forward.Z*Up.Y,Forward.Z*Up.X-Forward.X*Up.Z,Forward.X*Up.Y-Forward.Y*Up.X);}};
enum ECreatureType{NOT_HUMAN=0,HUMAN_CHILD=1,HUMAN_ADULT=2,HUMAN_ELDERLY=3};
struct CFaction{int id;};
struct CTCEnemy{const CFaction* faction;const CFaction* GetPFaction()const{return faction;}};
struct CTCPhysicsBase{CRightHandedSet axes;const CRightHandedSet& GetRHSet()const{return axes;}};
enum{TCI_ENEMY=11,TCI_PHYSICS=1};
struct CThingAICreature{C3DVector pos;ECreatureType type;CTCEnemy* enemy;CTCPhysicsBase* physics;mutable int positions,types,enemies,phys;
 const C3DVector& GetPos()const{++positions;return pos;}ECreatureType GetCreatureType()const{++types;return type;}
 bool PeekTC(int id,const CTCEnemy** p)const{check(id==TCI_ENEMY);++enemies;*p=enemy;return true;}
 bool PeekTC(int id,const CTCPhysicsBase** p)const{check(id==TCI_PHYSICS);++phys;*p=physics;return true;}
};
struct CThingFilter_IsAICreatureMatchingTypeAndFactionNearby{bool RestrictAngle;C3DVector OurPos;float MaxSqDist;C3DVector SideVec;ECreatureType OurCreatureType;const CFaction* PFaction;float MinCosine;
 CThingFilter_IsAICreatureMatchingTypeAndFactionNearby(const CThingAICreature&,float,bool);
};
CThingFilter_IsAICreatureMatchingTypeAndFactionNearby::CThingFilter_IsAICreatureMatchingTypeAndFactionNearby(const CThingAICreature& creature,float max_dist,bool restrict_angle)
{
 MinCosine=0.0f;
 RestrictAngle=restrict_angle;
 OurPos=creature.GetPos();
 MaxSqDist=max_dist*max_dist;
 OurCreatureType=creature.GetCreatureType();
 const CTCEnemy* ptc_enemy;
 creature.PeekTC(TCI_ENEMY,&ptc_enemy);
 PFaction=ptc_enemy->GetPFaction();
 if(restrict_angle) {
  const CTCPhysicsBase* ptc_physics;
  creature.PeekTC(TCI_PHYSICS,&ptc_physics);
  SideVec=ptc_physics->GetRHSet().GetRight();
  if(OurCreatureType==HUMAN_CHILD) SideVec=-SideVec;
  MinCosine=(float)cos(0.78539818525314331);
 }
}

int main(){
 check(offsetof(CThingFilter_IsAICreatureMatchingTypeAndFactionNearby,OurPos)==1);
#ifdef _MSC_VER
 check(sizeof(CThingFilter_IsAICreatureMatchingTypeAndFactionNearby)==44);
 check(offsetof(CThingFilter_IsAICreatureMatchingTypeAndFactionNearby,SideVec)==20);
 check(offsetof(CThingFilter_IsAICreatureMatchingTypeAndFactionNearby,PFaction)==36);
#endif
 CFaction faction;CTCEnemy enemy;CTCPhysicsBase physics;CThingAICreature creature;creature.enemy=&enemy;creature.physics=&physics;
 int cases=0;
 for(int type=0;type<4;++type)for(int restrict_angle=0;restrict_angle<2;++restrict_angle)for(int dist=-4;dist<=4;++dist)for(int basis=0;basis<3;++basis){
  enemy.faction=(basis&1)?&faction:0;creature.type=ECreatureType(type);creature.pos=C3DVector(float(basis*2),float(-basis),float(basis+3));
  physics.axes.Up=C3DVector(0,1,1);physics.axes.Forward=C3DVector(float(basis+1),2,-1);
  creature.positions=creature.types=creature.enemies=creature.phys=0;
  union Storage{double align;unsigned char data[80];}storage;std::memset(storage.data,0x5a,sizeof(storage.data));
  CThingFilter_IsAICreatureMatchingTypeAndFactionNearby* f=new(storage.data)CThingFilter_IsAICreatureMatchingTypeAndFactionNearby(creature,float(dist),restrict_angle!=0);
  check(f->RestrictAngle==(restrict_angle!=0)&&f->OurPos.X==creature.pos.X&&f->OurPos.Y==creature.pos.Y&&f->OurPos.Z==creature.pos.Z);
  check(f->MaxSqDist==float(dist*dist)&&f->OurCreatureType==type&&f->PFaction==enemy.faction);
  check(creature.positions==1&&creature.types==1&&creature.enemies==1&&creature.phys==restrict_angle);
  if(restrict_angle){
   const C3DVector& a=physics.axes.Forward;const C3DVector& b=physics.axes.Up;float sign=type==HUMAN_CHILD?-1.0f:1.0f;
   check(f->SideVec.X==(a.Y*b.Z-a.Z*b.Y)*sign&&f->SideVec.Y==(a.Z*b.X-a.X*b.Z)*sign&&f->SideVec.Z==(a.X*b.Y-a.Y*b.X)*sign);
   check(std::fabs(f->MinCosine-0.70710676908493042f)<0.000001f);
  }else{unsigned char pattern[12];std::memset(pattern,0x5a,12);check(!std::memcmp(pattern,&f->SideVec,12)&&f->MinCosine==0);}
  f->~CThingFilter_IsAICreatureMatchingTypeAndFactionNearby();++cases;
 }
 check(cases==216);std::puts("00986ed0_TEST PASS");return 0;
}
