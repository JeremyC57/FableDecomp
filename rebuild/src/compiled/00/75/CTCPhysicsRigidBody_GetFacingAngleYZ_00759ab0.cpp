// CTCPhysicsRigidBody::GetFacingAngleYZ() const -- return facing as a fraction of one turn wrapped into [0,1).
// Leaf: ?GetFacingAngleYZ@CTCPhysicsRigidBody@@UBEMXZ
// Identity: proven; PDB const GetFacingAngleYZ returns float; physics virtual entries and CCamera callers; RHSet.Forward component offsets retail 0x74/78/7C.
// Layout: generated physics headers use a flattened vptr with the wrong struct key and opaque RHSet bytes; local typed real base/member chain uses CRightHandedSet::Up then Forward. RigidBody three retail vectors are 12 bytes each; donor16-byte vectors account for the RHSet displacement. Camera/vector/right-handed-set have no generated header.
#define CTCPhysicsRigidBody CTCPhysicsRigidBody_HeaderLayout
#include "engine/CTCPhysicsRigidBody.h"
#undef CTCPhysicsRigidBody
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
class CTCPhysicsRigidBody:public CTCPhysicsBase { public: std::vector<C3DVector> CollisionNormal; std::vector<long> CollidingVertexIndex; std::vector<CThing*> Collidors; CThingPhysicsMeshInfo* PPhysicsMeshInfo; CRightHandedSet RHSet; virtual float GetFacingAngleYZ() const; };
float CTCPhysicsRigidBody::GetFacingAngleYZ() const { float angle=(float)atan2(RHSet.Forward.Z,sqrt(RHSet.Forward.Y*RHSet.Forward.Y+RHSet.Forward.X*RHSet.Forward.X))*0.15915494f; return WrapAngle(angle); }