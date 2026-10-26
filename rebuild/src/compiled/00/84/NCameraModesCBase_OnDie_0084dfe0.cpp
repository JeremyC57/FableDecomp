// NCameraModes::CBase::Reset() -- restore default camera dynamics and reset subcomponents.
// Leaf: Reset
// Retail 0x0084DFE0,194 bytes. Manifest CFirstPerson::OnDie is propagated incorrectly.
// Layout: no engine CBase header; donor CPhysicsState lists are 12 bytes, retail lists 4 bytes,
// so each physics state is 0x30. Reuses landed 0084CF80 member layout.
// Call 0084F280 is for_each counted subcomponent + slot1 Reset; audited native body and PDB specialization.
// Donor manager Camera +4 and OldCamera +3c agree; donor SingleCut +bc moves to native+b0.
#define CGameCameraManager CGameCameraManagerDonor
#include "engine/CGameCameraManager.h"
#undef CGameCameraManager
class C3DVector { public:
 float X,Y,Z;
 C3DVector() {}
 C3DVector(const float& x,const float& y,const float& z):X(x),Y(y),Z(z){}
 C3DVector(const C3DVector& s):X(s.X),Y(s.Y),Z(s.Z){}
};
class CCamera { public: unsigned long Words[14]; };
class CGameCameraManager { public:
 void* VTable; CCamera Camera,OldCamera;
 unsigned char OtherState[0x3c];
 bool SingleCut; // retail +0xb0: donor +0xbc minus STL shrink
};
struct CPhysicsState {
 C3DVector Position,Velocity,Acceleration;
 void* Attractors; void* IncidentalAttractors; bool CullPhysics;
};
struct CSubComponentBase;
struct CCountedSubComponent {CSubComponentBase* Data;void* Info;};
struct CCallResetMFInSubComponent {};
CCallResetMFInSubComponent __fastcall ForEachReset(CCountedSubComponent*,CCountedSubComponent*,CCallResetMFInSubComponent); //0x0084F280
namespace NCameraModes {
class CBase { public:
 void* VTable;float DefaultFOV,DefaultRollAngle,DefaultObservedPositionZOffset;
 CPhysicsState LookAt; CGameCameraManager* CameraManager;void* PCameraModeDef;
 CCountedSubComponent *SubStart,*SubFinish,*SubEnd;
 CPhysicsState GoTo;float CameraFOV,RollAngle,ObservedPositionZOffset;
 float RightStickVector[2],LeftStickVector[2];bool ViewOccluded;
 void Reset();
};
void CBase::Reset() {
 CGameCameraManager* manager=CameraManager;
 manager->SingleCut=true;
 manager->OldCamera=manager->Camera;
 GoTo.Velocity=C3DVector(0.0f,0.0f,0.0f);
 LookAt.Velocity=C3DVector(0.0f,0.0f,0.0f);
 DefaultFOV=0.2f;CameraFOV=0.2f;
 GoTo.CullPhysics=true;LookAt.CullPhysics=true;
 DefaultRollAngle=0;RollAngle=0;DefaultObservedPositionZOffset=0;ObservedPositionZOffset=0;
 ForEachReset(SubStart,SubFinish,CCallResetMFInSubComponent());
}
}