#ifndef FABLE_ENGINE_NCAMERAMODES_CPHYSICSCOORDRIGID_H
#define FABLE_ENGINE_NCAMERAMODES_CPHYSICSCOORDRIGID_H
#include "engine/NCameraModes_CPhysicsState.h"
#include "engine/CGameCameraManager.h"
namespace NCameraModes {
class CBase;
class CPhysicsCoordModelBase {
public:
 virtual void FrameUpdate();
protected:
 const C3DVector& GetAcceleration()const{return LinkedState.Acceleration;}
 const C3DVector& GetPosition()const{return LinkedState.Position;}
 void SetPosition(const C3DVector& v){LinkedState.Position=v;}
 void SetVelocity(const C3DVector& v){LinkedState.Velocity=v;}
 const CGameCameraManager& GetCameraManager(){return CameraManager;}
 CPhysicsState& LinkedState;
 CBase& Owner;
 const CGameCameraManager& CameraManager;
};
class CPhysicsCoordRigid:public CPhysicsCoordModelBase {
public:virtual void FrameUpdate();
};
}
FABLE_STATIC_ASSERT(sizeof(NCameraModes::CPhysicsCoordModelBase)==16);
FABLE_STATIC_ASSERT(sizeof(NCameraModes::CPhysicsCoordRigid)==16);
#endif
