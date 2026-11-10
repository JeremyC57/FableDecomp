#include <math.h>
#include <stdio.h>
#include <assert.h>
#ifndef _MSC_VER
#define __fastcall
#endif
class C3DVector {public:float X,Y,Z;C3DVector(){}C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}C3DVector(const C3DVector& v):X(v.X),Y(v.Y),Z(v.Z){}const C3DVector operator+(const C3DVector& v)const{return C3DVector(X+v.X,Y+v.Y,Z+v.Z);}const C3DVector operator*(float f)const{return C3DVector(X*f,Y*f,Z*f);}};
class C3DBoundingBox {C3DVector MinCoord,MaxCoord;public:C3DBoundingBox(const C3DVector&a,const C3DVector&b):MinCoord(a),MaxCoord(b){}const C3DVector GetCenter()const{return (MinCoord+MaxCoord)*0.5f;}};
class C3DMeshInfo {C3DBoundingBox BoundingBox;public:C3DMeshInfo():BoundingBox(C3DVector(-2,-3,-4),C3DVector(4,5,8)){}C3DBoundingBox GetBoundingBox()const{return BoundingBox;}};
class CMeshDataBank{public:C3DMeshInfo mesh;const C3DMeshInfo* PeekMeshInfo(unsigned long index)const{assert(index==7);return &mesh;}};
class CWorld{public:CMeshDataBank bank;CMeshDataBank* GetMeshBank(){return &bank;}};
class CMainGameComponent{public:static long fps;static long __fastcall GetConstantFPS(){return fps;}};long CMainGameComponent::fps=30;
class CHighlightItemDef{public:long refs;float RotationSpeed,DistanceFromGround,BobDistance,BobTime;};
template<class T>class CDefPointer{T* Object;public:CDefPointer():Object(0){}~CDefPointer(){if(Object)--const_cast<CHighlightItemDef*>(Object)->refs;}void Assign(T*p){Object=p;if(Object)++const_cast<CHighlightItemDef*>(Object)->refs;}T*operator->()const{return Object;}};
class CThingBaseDef{public:CHighlightItemDef* value;bool GetSubDef(CDefPointer<const CHighlightItemDef>*p)const{p->Assign(value);return true;}};
static int order;
class CRightHandedSet{public:float angle;C3DVector axis;CRightHandedSet():angle(0),axis(0,0,0){}void Rotate(const C3DVector&a,float f){assert(order==2);order=3;axis=a;angle=f;}};
class CTCPhysicsBase{C3DVector Position;CRightHandedSet rhs;public:CTCPhysicsBase():Position(3,4,10){}const C3DVector& GetPosition()const{return Position;}void SetPosition(const C3DVector&p){assert(order==0);order=1;Position=p;}const CRightHandedSet& GetRHSet()const{assert(order==1);order=2;return rhs;}void SetRHSet(const CRightHandedSet&r){assert(order==3);order=4;rhs=r;}void SetAsMoving(bool b){assert(b&&order==4);order=5;}const C3DVector& Result()const{return Position;}const CRightHandedSet& Rotation()const{return rhs;}};
class CEngineGraphic{public:unsigned long BankIndex;float AnimStep,RenderSizeX;bool AdditiveAlpha;unsigned char Type;};
namespace NGraphicAppearance{class CTCGraphicAppearance{public:CEngineGraphic graphic;float Scale;const CEngineGraphic& GetMainGraphic()const{return graphic;}float GetScale()const{return Scale;}};}
class CThing{public:CThingBaseDef def;CTCPhysicsBase physics;NGraphicAppearance::CTCGraphicAppearance appearance;const CThingBaseDef& PeekBaseDef()const{return def;}CTCPhysicsBase* GetPhysicsTC(){return &physics;}NGraphicAppearance::CTCGraphicAppearance* GetTCGraphicAppearance(){return &appearance;}};
class CWorldMap{public:class CThingFilterBase{public:virtual bool operator()(const CThing&)const{return true;}};float ground;float GetHeightZBelowAt(const C3DVector&p,float offset,bool b,const CThingFilterBase&)const{assert(p.X==11&&p.Y==12&&p.Z==13&&offset==0&&!b);return ground;}};
class CTCHighlightItem{public:CThing thing;CWorld world;CWorldMap map;long LifeTimer;C3DVector StartPos;CThing&GetParentThing(){return thing;}const CWorldMap&PeekWorldMap()const{return map;}CWorld&GetWorld(){return world;}void DoMovement();};
inline double __fastcall GFCos(float angle){return cos(angle*(double)6.2831855f);}
inline float __fastcall GFGetWeightedFractionSlowStartSlowEnd(float fraction){fraction=(float)((1.0-GFCos(fraction*0.5f))/2.0);return fraction;}
void CTCHighlightItem::DoMovement(){
 CDefPointer<const CHighlightItemDef> pdef;
 GetParentThing().PeekBaseDef().GetSubDef(&pdef);
 CTCPhysicsBase* ptc_physics=GetParentThing().GetPhysicsTC();
 C3DVector current_pos=ptc_physics->GetPosition();
 float ground_z=PeekWorldMap().GetHeightZBelowAt(StartPos,0.0f,false,CWorldMap::CThingFilterBase());
 C3DVector new_pos=current_pos;
 float bob_time_turns=CMainGameComponent::GetConstantFPS()*pdef->BobTime;
 float half_bob_time_turns=bob_time_turns*0.5f;
 float rounded_life_timer=(float)fmod((float)LifeTimer,bob_time_turns);
 float fraction;
 if(rounded_life_timer>half_bob_time_turns)fraction=1.0f-GFGetWeightedFractionSlowStartSlowEnd((rounded_life_timer-half_bob_time_turns)/half_bob_time_turns);
 else fraction=GFGetWeightedFractionSlowStartSlowEnd(rounded_life_timer/half_bob_time_turns);
 long graphic_index=GetParentThing().GetTCGraphicAppearance()->GetMainGraphic().BankIndex;
 float scale=GetParentThing().GetTCGraphicAppearance()->GetMainGraphic().RenderSizeX*GetParentThing().GetTCGraphicAppearance()->GetScale()*0.01f;
 float centre_z_offset=GetWorld().GetMeshBank()->PeekMeshInfo(graphic_index)->GetBoundingBox().GetCenter().Z*scale;
 float min_distance_from_ground=pdef->DistanceFromGround-pdef->BobDistance*0.5f;
 new_pos.Z=min_distance_from_ground+fraction*pdef->BobDistance+ground_z-centre_z_offset;
 ptc_physics->SetPosition(new_pos);
 CRightHandedSet rhs=ptc_physics->GetRHSet();
 rhs.Rotate(C3DVector(0.0f,0.0f,1.0f),pdef->RotationSpeed*(1.0f/360.0f));
 ptc_physics->SetRHSet(rhs);
 ptc_physics->SetAsMoving(true);
}
int main(){for(int frames=0;frames<120;++frames)for(int scale=1;scale<=3;++scale)for(int period=1;period<=2;++period){CTCHighlightItem item;CHighlightItemDef def;def.refs=0;def.RotationSpeed=90;def.DistanceFromGround=5;def.BobDistance=2;def.BobTime=(float)period;item.thing.def.value=&def;item.LifeTimer=frames;item.StartPos=C3DVector(11,12,13);item.map.ground=3;item.thing.appearance.Scale=(float)scale;item.thing.appearance.graphic.BankIndex=7;item.thing.appearance.graphic.RenderSizeX=50;order=0;item.DoMovement();assert(order==5&&def.refs==0);double total=30.0*period,half=total*.5,phase=fmod((double)frames,total);double fraction=phase>half?1-(1-cos((phase-half)/half*.5*(double)6.2831855f))*.5:(1-cos(phase/half*.5*(double)6.2831855f))*.5;double z=5-1+fraction*2+3-2*(50*scale*.01);assert(fabs(item.thing.physics.Result().Z-z)<0.00001);assert(item.thing.physics.Result().X==3&&item.thing.physics.Result().Y==4);assert(fabs(item.thing.physics.Rotation().angle-.25)<0.000001);assert(item.thing.physics.Rotation().axis.Z==1);}puts("0077c960_TEST PASS");return 0;}