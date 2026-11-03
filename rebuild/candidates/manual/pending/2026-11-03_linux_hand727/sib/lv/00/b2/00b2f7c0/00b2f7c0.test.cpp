// CEnginePrimitiveOcclusionManager::FrameStart -- invalidate occlusion on a new scene or excessive camera translation/turn, then remember current camera position/direction. Retail00b2f7c0 complete230bytes ret4,10int3 padding. Actual shared manager/camera headers; reinterpret flattened C3DVector members via native threefloat view. CameraDesc WorldPosition+40 (camera+54),LookVector+1b4. Angle threshold uses doubleturn-to-radian pool0124f2b8 and intrinsiccos.
#include <math.h>
#ifdef _MSC_VER
#include "engine/CEnginePrimitiveOcclusionManager.h"
#include "engine/CEngineCamera.h"
#else
class CEnginePrimitiveOcclusionManager { public:unsigned long PrimitiveOccludeDelay;long ShowVisibilityInfo;float LastCameraDirection_X,LastCameraDirection_Y,LastCameraDirection_Z;float LastCameraPosition_X,LastCameraPosition_Y,LastCameraPosition_Z;float MaxCameraAngleStep,MaxCameraPositionStep;bool InvalidateOcclusion,NewSceneFlag;};
class CEngineCamera { public:unsigned char CameraDesc[0x6c];float LookVector_X,LookVector_Y,LookVector_Z;};
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
class CEngineComponentParams;
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

#include <cstdio>
class CEngineComponentParams { public:int Marker;};
CEngineCamera* g_pEngineCamera=0;
static int failures=0;
static void check(bool b){if(!b)++failures;}
static void run(bool newScene,float x,float y,float z,float maxPosition,float maxAngle,float dot,bool expect)
{
 CEnginePrimitiveOcclusionManager_Methods manager;
 manager.PrimitiveOccludeDelay=13;manager.ShowVisibilityInfo=-17;manager.LastCameraPosition_X=10;manager.LastCameraPosition_Y=20;manager.LastCameraPosition_Z=30;manager.LastCameraDirection_X=1;manager.LastCameraDirection_Y=0;manager.LastCameraDirection_Z=0;manager.MaxCameraPositionStep=maxPosition;manager.MaxCameraAngleStep=maxAngle;manager.NewSceneFlag=newScene;manager.InvalidateOcclusion=true;
 CEngineCamera camera;g_pEngineCamera=&camera;
 CCameraDescriptionView* desc=reinterpret_cast<CCameraDescriptionView*>(camera.CameraDesc);desc->WorldPosition.X=x;desc->WorldPosition.Y=y;desc->WorldPosition.Z=z;
 camera.LookVector_X=dot;camera.LookVector_Y=0.6f;camera.LookVector_Z=-0.3f;
 CEngineComponentParams params;params.Marker=975;manager.FrameStart(params);
 check(manager.InvalidateOcclusion==expect && !manager.NewSceneFlag && params.Marker==975);
 check(manager.LastCameraPosition_X==x && manager.LastCameraPosition_Y==y && manager.LastCameraPosition_Z==z);
 check(manager.LastCameraDirection_X==dot && manager.LastCameraDirection_Y==0.6f && manager.LastCameraDirection_Z==-0.3f);
 check(manager.PrimitiveOccludeDelay==13 && manager.ShowVisibilityInfo==-17 && manager.MaxCameraPositionStep==maxPosition && manager.MaxCameraAngleStep==maxAngle);
 // A second unchanged frame must clear invalidation and retain the newly saved values.
 camera.LookVector_X=1;camera.LookVector_Y=0;camera.LookVector_Z=0;manager.LastCameraDirection_X=1;manager.LastCameraDirection_Y=0;manager.LastCameraDirection_Z=0;manager.MaxCameraAngleStep=0.1f;manager.MaxCameraPositionStep=1;manager.FrameStart(params);check(!manager.InvalidateOcclusion);
}
int main(){run(true,10,20,30,100,0.2f,1,true);run(false,10,20,30,0,0,1,false);run(false,13,24,42,13,0.2f,1,false);run(false,13,24,42,12.99f,0.2f,1,true);run(false,10,20,30,10,0.2f,0.70710678f,false);run(false,10,20,30,10,0.125f,0.5f,true);run(false,10,20,30,10,0,0.999f,true);run(false,10,20,30,10,0.25f,0,false);if(failures){std::printf("00b2f7c0_TEST FAIL %d\n",failures);return 1;}std::printf("00b2f7c0_TEST PASS\n");return 0;}