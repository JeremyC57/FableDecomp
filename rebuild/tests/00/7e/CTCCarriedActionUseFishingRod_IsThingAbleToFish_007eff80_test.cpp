#include <cassert>
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define FABLE_FASTCALL
#else
#define FABLE_FASTCALL __fastcall
#endif
static int FacingQueries,PosQueries,HeightQueries,ComponentQueries,WaterQueries,GroundQueries,CameraQueries,FocalQueries,LOSQueries;
static unsigned int GFastOneOverSqrtTable[128];
class C3DVector {public:float X,Y,Z;C3DVector(){}C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}C3DVector operator+(const C3DVector& v)const{return C3DVector(X+v.X,Y+v.Y,Z+v.Z);}C3DVector& operator*=(float f){X*=f;Y*=f;Z*=f;return *this;}
 C3DVector& Normalise(){float value=X*X+Y*Y+Z*Z;unsigned int bits;std::memcpy(&bits,&value,4);unsigned int estimate=(((0x17c-((bits>>23)&0xff))>>1)<<23)|(GFastOneOverSqrtTable[(bits>>17)&0x7f]<<15);float r;std::memcpy(&r,&estimate,4);float one_on_magnitude=(3.0f-r*r*value)*r*0.5f;X*=one_on_magnitude;Y*=one_on_magnitude;Z*=one_on_magnitude;return *this;}
};
static bool Same(const C3DVector& a,const C3DVector& b){return a.X==b.X&&a.Y==b.Y&&a.Z==b.Z;}
class CFishingDef {public:mutable unsigned long RefCount;float FishingSpotMinDepth,FishingSpotMinHeightDiff,FishingSpotMaxHeightDiff;};
template<class T>class CDefPointer {public:T* Object;CDefPointer():Object(0){}CDefPointer(const CDefPointer& p):Object(p.Object){if(Object)++Object->RefCount;}~CDefPointer(){if(Object)--Object->RefCount;}T* operator->()const{return Object;}};
class CTCFishing {public:CDefPointer<const CFishingDef> PDef;CDefPointer<const CFishingDef> GetPDef()const{return PDef;}};
class CThing {public:C3DVector Position;};
namespace NCameraModes {
class CTools {public:enum ELOSTest {TEST_ALL_BUT_CREATURES=7};C3DVector WantFrom,WantTo;bool Clear;
 bool IsLineOfSight(const C3DVector& from,const C3DVector& to,C3DVector& collision_pos,C3DVector& normal,float& radius,const CThing** hit,ELOSTest test,bool flag)const{++LOSQueries;assert(Same(from,WantFrom)&&Same(to,WantTo)&&test==TEST_ALL_BUT_CREATURES&&!flag);collision_pos=to;normal=C3DVector(0,0,1);radius=0.25f;*hit=0;return Clear;}
};
}
class CGameCameraManager {public:NCameraModes::CTools Tools;const NCameraModes::CTools& GetCameraTools()const{return Tools;}};
class CAIGameCameraBase {public:const CGameCameraManager* Manager;virtual bool GetCameraManager(const CGameCameraManager** p)const{++CameraQueries;*p=Manager;return true;}};
namespace NGraphicAppearance {class CTCGraphicAppearance {public:C3DVector Focal;C3DVector GetStaticFocalPos()const{++FocalQueries;return Focal;}};}
class CTCPhysicsBase {public:C3DVector Facing;virtual C3DVector GetFacingVector()const{++FacingQueries;return Facing;}};
class CThingCreatureBase:public CThing {public:CTCPhysicsBase* Physics;const CTCFishing* Fishing;const NGraphicAppearance::CTCGraphicAppearance* Appearance;C3DVector WantCheck;float CheckHeight;
 const CTCPhysicsBase* GetPhysicsTC()const{return Physics;}const C3DVector& GetPos()const{++PosQueries;return Position;}float GetHeightZBelowAt(const C3DVector& p,bool a,bool b)const{++HeightQueries;assert(Same(p,WantCheck)&&a&&b);return CheckHeight;}
 bool PeekTC(const CTCFishing** p)const{++ComponentQueries;assert(Fishing);*p=Fishing;return true;}const NGraphicAppearance::CTCGraphicAppearance* GetTCGraphicAppearance()const{assert(Appearance);return Appearance;}
};
class CWorldMap {public:C3DVector WantWaterPos,WantFisherPos;float Water,WaterGround,FisherGround;float GetWaterSizeZAt(const C3DVector& p)const{++WaterQueries;assert(Same(p,WantWaterPos));return Water;}float GetGroundSizeZAt(const C3DVector& p)const{++GroundQueries;if(GroundQueries==1){assert(Same(p,WantWaterPos));return WaterGround;}assert(GroundQueries==2&&Same(p,WantFisherPos));return FisherGround;}};
class CWorld {public:CWorldMap* Map;const CAIGameCameraBase* Camera;virtual const CWorldMap& PeekWorldMap()const{return *Map;}const CAIGameCameraBase* PeekGameCamera()const{assert(Camera);return Camera;}};
class CMainGameComponent {public:static CMainGameComponent* Current;CWorld* World;static CMainGameComponent* Get(){return Current;}CWorld& GetWorld(){return *World;}};CMainGameComponent* CMainGameComponent::Current;
class CTCCarriedActionUseFishingRod {public:static bool FABLE_FASTCALL IsThingAbleToFish(const CThingCreatureBase&);};

bool FABLE_FASTCALL CTCCarriedActionUseFishingRod::IsThingAbleToFish(const CThingCreatureBase& fisher)
{
    C3DVector facing_dir=fisher.GetPhysicsTC()->GetFacingVector();
    facing_dir.Normalise();
    facing_dir*=4.0f;
    C3DVector fishing_pos=fisher.GetPos()+facing_dir;
    const CWorldMap& world_map=CMainGameComponent::Get()->GetWorld().PeekWorldMap();
    float water_depth=world_map.GetWaterSizeZAt(fishing_pos);
    if(water_depth>0.05f) {
        C3DVector check_pos=fishing_pos;
        check_pos.Z+=0.5f;
        float check_depth=fisher.GetHeightZBelowAt(check_pos,true,true);
        if(check_depth<water_depth) {
            const CTCFishing* ptc_fishing;
            fisher.PeekTC(&ptc_fishing);
            CDefPointer<const CFishingDef> pdef=ptc_fishing->GetPDef();
            float ground_height=world_map.GetGroundSizeZAt(fishing_pos);
            if(water_depth-ground_height>pdef->FishingSpotMinDepth) {
                float height_diff=world_map.GetGroundSizeZAt(fisher.GetPos())-water_depth;
                if(height_diff>=pdef->FishingSpotMinHeightDiff&&height_diff<=pdef->FishingSpotMaxHeightDiff) {
                    const CGameCameraManager* pcam_man;
                    CMainGameComponent::Get()->GetWorld().PeekGameCamera()->GetCameraManager(&pcam_man);
                    const CThing* phit_thing;
                    C3DVector normal_vector;
                    float camera_radius;
                    C3DVector collision_pos;
                    if(pcam_man->GetCameraTools().IsLineOfSight(fisher.GetTCGraphicAppearance()->GetStaticFocalPos(),check_pos,collision_pos,normal_vector,camera_radius,&phit_thing,NCameraModes::CTools::TEST_ALL_BUT_CREATURES,false))
                        return true;
                }
            }
        }
    }
    return false;
}

int main()
{
 // Controlled external seed table: powers-of-four squared lengths exercise a fixed mantissa seed.
 // This fixture supplies the external dependency; it does not reproduce or export game table data.
 std::memset(GFastOneOverSqrtTable,0,sizeof GFastOneOverSqrtTable);GFastOneOverSqrtTable[64]=255;
 const C3DVector directions[]={C3DVector(1,0,0),C3DVector(0,2,0),C3DVector(0,0,4),C3DVector(-1,0,0)};
 const C3DVector unit[]={C3DVector(1,0,0),C3DVector(0,1,0),C3DVector(0,0,1),C3DVector(-1,0,0)};
 const float normalized=0.9999942779541015625f;
 const float waters[]={-1.0f,0.0f,0.03125f,0.0625f,4.0f,8.0f};const float checks[]={-0.5f,0.0f,0.5f};const float depths[]={0.5f,1.0f,1.5f};const float heights[]={0.25f,0.5f,1.25f,2.0f,2.25f};int cases=0;
 for(int direction=0;direction<4;++direction)for(int water=0;water<6;++water)for(int check=0;check<3;++check)for(int depth=0;depth<3;++depth)for(int height=0;height<5;++height)for(int clear=0;clear<2;++clear)for(int refs=0;refs<2;++refs){
  FacingQueries=PosQueries=HeightQueries=ComponentQueries=WaterQueries=GroundQueries=CameraQueries=FocalQueries=LOSQueries=0;
  CFishingDef def;def.RefCount=2+refs;def.FishingSpotMinDepth=1.0f;def.FishingSpotMinHeightDiff=0.5f;def.FishingSpotMaxHeightDiff=2.0f;
  CTCFishing fishing;fishing.PDef.Object=&def;CTCPhysicsBase physics;physics.Facing=directions[direction];NGraphicAppearance::CTCGraphicAppearance appearance;appearance.Focal=C3DVector(3,5,7);
  CThingCreatureBase fisher;fisher.Position=C3DVector(8,16,32);fisher.Physics=&physics;fisher.Fishing=&fishing;fisher.Appearance=&appearance;C3DVector want(fisher.Position.X+(4*normalized)*unit[direction].X,fisher.Position.Y+(4*normalized)*unit[direction].Y,fisher.Position.Z+(4*normalized)*unit[direction].Z);fisher.WantCheck=C3DVector(want.X,want.Y,want.Z+0.5f);fisher.CheckHeight=waters[water]+checks[check];
  CWorldMap map;map.WantWaterPos=want;map.WantFisherPos=fisher.Position;map.Water=waters[water];map.WaterGround=waters[water]-depths[depth];map.FisherGround=waters[water]+heights[height];
  CGameCameraManager manager;manager.Tools.WantFrom=appearance.Focal;manager.Tools.WantTo=fisher.WantCheck;manager.Tools.Clear=clear!=0;CAIGameCameraBase camera;camera.Manager=&manager;CWorld world;world.Map=&map;world.Camera=&camera;CMainGameComponent game;game.World=&world;CMainGameComponent::Current=&game;
  bool wet=water>=3,under=wet&&check==0,deep=under&&depth==2,inrange=deep&&height>=1&&height<=3;
  // Early failure never needs fishing or camera dependencies.
  if(!under)fisher.Fishing=0;if(!inrange){world.Camera=0;fisher.Appearance=0;}
  bool result=CTCCarriedActionUseFishingRod::IsThingAbleToFish(fisher);
  assert(result==(inrange&&clear!=0));assert(FacingQueries==1&&WaterQueries==1&&HeightQueries==(wet?1:0)&&ComponentQueries==(under?1:0));assert(GroundQueries==(under?(depth==2?2:1):0));assert(PosQueries==1+(deep?1:0));assert(CameraQueries==(inrange?1:0)&&FocalQueries==(inrange?1:0)&&LOSQueries==(inrange?1:0));
  assert(def.RefCount==(unsigned long)(2+refs)&&def.FishingSpotMinDepth==1.0f&&def.FishingSpotMinHeightDiff==0.5f&&def.FishingSpotMaxHeightDiff==2.0f);assert(Same(physics.Facing,directions[direction])&&fisher.Position.X==8&&fisher.Position.Y==16&&fisher.Position.Z==32);++cases;
 }
 assert(cases==4320);std::puts("007eff80_TEST PASS");return 0;
}