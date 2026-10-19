// CEnginePrimitiveOcclusionManager::FrameStart -- invalidate occlusion on a new scene or excessive camera translation/turn, then remember current camera position/direction. Retail00b2f7c0 complete230bytes ret4,10int3 padding. Actual shared manager/camera headers; reinterpret flattened C3DVector members via native threefloat view. CameraDesc WorldPosition+40 (camera+54),LookVector+1b4. Angle threshold uses doubleturn-to-radian pool0124f2b8 and intrinsiccos.
#include <math.h>
#ifdef _MSC_VER
#include "engine/CEnginePrimitiveOcclusionManager.h"
#include "engine/CEngineCamera.h"
#else
struct CEnginePrimitiveOcclusionManager {unsigned long PrimitiveOccludeDelay;long ShowVisibilityInfo;float LastCameraDirection_X,LastCameraDirection_Y,LastCameraDirection_Z;float LastCameraPosition_X,LastCameraPosition_Y,LastCameraPosition_Z;float MaxCameraAngleStep,MaxCameraPositionStep;bool InvalidateOcclusion,NewSceneFlag;};
struct CEngineCamera {unsigned char CameraDesc[0x6c];float LookVector_X,LookVector_Y,LookVector_Z;};
#endif
struct CCameraVector {
 float X,Y,Z;
 CCameraVector(float x,float y,float z):X(x),Y(y),Z(z){}
 CCameraVector(const CCameraVector& o):X(o.X),Y(o.Y),Z(o.Z){}
 CCameraVector operator-(const CCameraVector& o)const{return CCameraVector(X-o.X,Y-o.Y,Z-o.Z);}
 float Length()const{return (float)sqrt(X*X+Y*Y+Z*Z);}
 float Dot(const CCameraVector& o)const{return X*o.X+Y*o.Y+Z*o.Z;}
};
struct CCameraDescriptionView {unsigned char Window[0x10];unsigned char RotationMatrix[0x30];CCameraVector WorldPosition;};
extern CEngineCamera* g_pEngineCamera; //01436ea0
struct CEngineComponentParams;
struct CEnginePrimitiveOcclusionManager_Methods:CEnginePrimitiveOcclusionManager {void FrameStart(const CEngineComponentParams&);};
void CEnginePrimitiveOcclusionManager_Methods::FrameStart(const CEngineComponentParams&)
{
 InvalidateOcclusion=false;
 CCameraVector position=reinterpret_cast<const CCameraDescriptionView*>(g_pEngineCamera->CameraDesc)->WorldPosition;
 CCameraVector direction=reinterpret_cast<const CCameraVector&>(g_pEngineCamera->LookVector_X);
 if(NewSceneFlag){NewSceneFlag=false;InvalidateOcclusion=true;}
 else if((position-reinterpret_cast<const CCameraVector&>(LastCameraPosition_X)).Length()>MaxCameraPositionStep || direction.Dot(reinterpret_cast<const CCameraVector&>(LastCameraDirection_X))<cos(MaxCameraAngleStep*(double)6.2831855f))InvalidateOcclusion=true;
 reinterpret_cast<CCameraVector&>(LastCameraPosition_X)=position;
 reinterpret_cast<CCameraVector&>(LastCameraDirection_X)=direction;
}