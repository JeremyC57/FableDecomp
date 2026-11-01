// 0087bf40 NCameraModes::CRotateAround::OnInit
// Leaf: ?OnInit@CRotateAround@NCameraModes@@MAEXXZ
// Flags:
// Bounds: assigned6-byte span is incomplete; diagnostic function length295 ends0087c067.
// Layout: original Ego_r CBase members; native CPhysicsState30 rather than donor40,
// vector storage0c rather than10. CBasea4, CRotateAroundc8; shared manager CurrentMode98.
#include <math.h>
#pragma intrinsic(sqrt)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"
#define CThing CThingFields
#include "engine/_quarantine/CThing.h"
#undef CThing
#define CThingGameObject CThingGameObjectFields
#include "engine/CThingGameObject.h"
#undef CThingGameObject
#define CGameCameraManager CGameCameraManagerFields
#include "engine/CGameCameraManager.h"
#undef CGameCameraManager
class C3DVector{public:float X,Y,Z;C3DVector(){}C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}C3DVector(const C3DVector&v):X(v.X),Y(v.Y),Z(v.Z){}C3DVector&AccurateNormalise();};
class C2DVector{public:float X,Y;};
class CThing:public CThingFields{public:const C3DVector&GetPos()const;};
// Layout: shared CThingGameObject storage extends CThing98 throughd0; no derived fields read.
class CThingGameObject:public CThing{unsigned char _pad_0x98[0x38];};
class CCameraModeDef;
// Layout: use shared handle storage and original same-type assignment; native reloads the source after Release.
template<> __forceinline CDefPointer<const CCameraModeDef>&CDefPointer<const CCameraModeDef>::operator=(const CDefPointer<const CCameraModeDef>&other){if(Object!=other.Object){Release();Object=other.Object;if(Object)Object->AddRef();}return *this;}
class CEnginePrimitivePrepareInfo;
namespace NCameraModes{class CBase;enum EMode{};}
class CGameCameraManager:public CGameCameraManagerFields{public:NCameraModes::CBase*GetCameraMode(){return reinterpret_cast<NCameraModes::CBase*>(CurrentMode_Data);}const NCameraModes::CBase*GetCameraMode()const{return reinterpret_cast<const NCameraModes::CBase*>(CurrentMode_Data);}};
namespace NCameraModes{
// Layout: native physics state Position0, Velocityc, Acceleration18, two4-byte lists24/28, flag2c.
// Only Position is read; the untouched suffix is retained as storage, not invented field types.
class CPhysicsState{public:C3DVector Position;unsigned char _pad_0x0c[0x24];};
class CBase{
public: virtual ~CBase();
public: virtual void Update()=0;
public: virtual NCameraModes::EMode GetModeType() const=0;
public: virtual void OnCameraInsideObservedThing();
public: virtual bool IsUseRightStickLookAround() const;
public: virtual void PreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&) const;
public: virtual bool IsShortenOnOcclusion() const;
public: virtual bool IsRaiseUpOnHeroNearby() const;
public: virtual bool IsStretchAroundHero() const;
public: virtual bool IsAvoidLandscape() const;
public: virtual bool IsFOVChange() const;
public: virtual bool IsTrackMeleeOpponents() const;
public: virtual bool IsTrackMeleeOpponentsAutoFOVChange() const;
public: virtual bool IsFinished() const;
public: virtual bool IsStringAndCage() const;
public: virtual float GetObservedThingCurrentMeshHeight() const;
public: virtual void Recentre();
public: virtual void UpdateSubComponents();
public: virtual float GetDefaultFOV() const;
public: virtual float GetDefaultRollAngle() const;
public: virtual float GetDefaultObservedPositionZOffset() const;
protected: virtual void OnInit()=0;
protected: virtual void OnDie()=0;
protected: virtual void OnReset();
protected:float DefaultFOV,DefaultRollAngle,DefaultObservedPositionZOffset;CPhysicsState LookAt;
private:CGameCameraManager&CameraManager;CDefPointer<const CCameraModeDef>PCameraModeDef;unsigned char SubComponents[12];CPhysicsState GoTo;float CameraFOV,RollAngle,ObservedPositionZOffset;C2DVector RightStickVector,LeftStickVector;bool ViewOccluded;
public:const CThingGameObject*GetObservedThing()const;const C3DVector&GetPosition()const{return GoTo.Position;}const C3DVector&GetObservedPos()const{return LookAt.Position;}float GetObservedPositionZOffset()const{return ObservedPositionZOffset;}void SetObservedPositionZOffset(float f){ObservedPositionZOffset=f;}const CDefPointer<const CCameraModeDef>&GetCameraModeDef()const{return PCameraModeDef;}
protected:CGameCameraManager&GetCameraManager(){return CameraManager;}const CGameCameraManager&GetCameraManager()const{return CameraManager;}void SetCameraModeDef(const CDefPointer<const CCameraModeDef>&p){PCameraModeDef=p;}
};
class CRotateAround:public CBase{
public:virtual EMode GetModeType()const;virtual void Update();virtual bool IsShortenOnOcclusion()const;virtual bool IsFOVChange()const;virtual bool IsAvoidLandscape()const;virtual float GetDefaultFOV()const;virtual float GetDefaultRollAngle()const;virtual float GetDefaultObservedPositionZOffset()const;
protected:virtual void OnInit();virtual void OnDie();
private:float StringLengthXY,HeightOffset;C3DVector XAxis,YAxis;float Theta;
};
FABLE_STATIC_ASSERT(sizeof(CBase)==0xa4);
FABLE_STATIC_ASSERT(sizeof(CRotateAround)==0xc8);

void CRotateAround::OnInit(){
 C3DVector axis;axis.X=LookAt.Position.X;axis.Y=LookAt.Position.Y;
 float dx=axis.X-GetPosition().X;float dy=axis.Y-GetPosition().Y;
 StringLengthXY=(float)sqrt(dx*dx+dy*dy);
 HeightOffset=GetPosition().Z-GetObservedThing()->GetPos().Z;
 axis.X=GetPosition().X-axis.X;axis.Y=GetPosition().Y-axis.Y;axis.Z=0.0f;
 C3DVector&xAxis=XAxis;xAxis=axis;xAxis.AccurateNormalise();
 axis.X=xAxis.Y;axis.Y=-xAxis.X;axis.Z=0.0f;YAxis=axis;YAxis.AccurateNormalise();Theta=0.0f;
 CBase*previous=GetCameraManager().GetCameraMode();DefaultFOV=previous->GetDefaultFOV();
 SetObservedPositionZOffset(previous->GetObservedPositionZOffset());SetCameraModeDef(previous->GetCameraModeDef());
}
}
