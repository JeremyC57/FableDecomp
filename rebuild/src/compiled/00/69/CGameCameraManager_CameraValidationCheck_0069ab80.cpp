// CGameCameraManager::CameraValidationCheck() -- reset and restore last camera if position or forward axis is NaN
// Retail0069AB80 173bytes. BFEC26 import thunk maps MSVCR71!_isnan at IAT014400E8.
// Reset0069A990 audited native manager routine. Camera has no generated header; native/PDB camera size38 and WorldPosition0 and RHSet.Forward18.
#define CGameCameraManager CGameCameraManagerDonor
#include "engine/CGameCameraManager.h"
#undef CGameCameraManager
struct C3DVector { float X,Y,Z; };
struct CRightHandedSet { C3DVector Up,Forward; };
struct CCamera { C3DVector WorldPosition; CRightHandedSet RHSet; bool HeightLocked,ViewVecZLocked; unsigned int FOVFlags; float HorizontalFOV,VerticalFOV,Zoom; };
extern "C" int _isnan(double); // 00BFEC26 import thunk
struct CGameCameraManager {
 void* VTable; CCamera Camera,OldCamera;
 void Reset(); //0069A990
 void CameraValidationCheck();
};
void CGameCameraManager::CameraValidationCheck(){
 CCamera* camera=&Camera;
 bool bad=false;
 int x=_isnan(Camera.WorldPosition.X),y=_isnan(Camera.WorldPosition.Y),z=_isnan(Camera.WorldPosition.Z);
 if(x||y||z)bad=true;
 x=_isnan(Camera.RHSet.Forward.X);y=_isnan(Camera.RHSet.Forward.Y);z=_isnan(Camera.RHSet.Forward.Z);
 if(x||y||z||bad){Reset();*camera=OldCamera;}
}