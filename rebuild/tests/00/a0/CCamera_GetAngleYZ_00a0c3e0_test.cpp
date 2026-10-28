#include <stdio.h>
#include <math.h>
#ifndef _MSC_VER
#define __forceinline inline
#endif
#pragma intrinsic(atan2,sqrt,fmod)
class C3DVector { public: float X,Y,Z; };
class CRightHandedSet { public: C3DVector Up,Forward; };
class CThing; class CPhysicsDef; class CThingPhysicsMeshInfo;
class CTCBase { public: virtual ~CTCBase() {} CThing* ParentThing; bool ToBeRemoved,IsInGlobalUpdateTCs; };
class CTCPhysicsBase:public CTCBase { public: C3DVector Position,OldPosition; long OldPositionLastFrameSet; C3DVector Velocity; float Radius; const CPhysicsDef* PhysicsDef; bool Moving:1; bool OldMoving:1; };
namespace std { template<class T> class vector { public: T* _M_start; T* _M_finish; T* _M_end_of_storage; }; }
__forceinline float WrapAngle(float angle) { if(angle<0.0f || angle>=1.0f) { angle=(float)fmod((double)angle,1.0); if(angle<0.0f) angle+=1.0f; } return angle; }
class CCamera { public: C3DVector WorldPosition; CRightHandedSet RHSet; float GetAngleYZ() const; };
float CCamera::GetAngleYZ() const { float angle=(float)atan2(RHSet.Forward.Z,sqrt(RHSet.Forward.Y*RHSet.Forward.Y+RHSet.Forward.X*RHSet.Forward.X))*0.15915494f; return WrapAngle(angle); }
int main() { CCamera object; C3DVector inputs[4]={{1,0,0},{0,1,1},{1,0,-1},{0,0,1}}; float expected[4]={0.0f,0.125f,0.875f,0.25f}; for(int i=0;i<4;++i) { object.RHSet.Forward=inputs[i]; float result=object.GetAngleYZ(); if(fabs(result-expected[i])>0.00001f || result<0 || result>=1) goto fail; }
 if(fabs(WrapAngle(-1.25f)-0.75f)>0.00001f || fabs(WrapAngle(2.25f)-0.25f)>0.00001f || WrapAngle(1.0f)!=0.0f) goto fail;
 puts("00a0c3e0_TEST PASS"); return 0; fail: puts("00a0c3e0_TEST FAIL"); return 1; }