// NCameraModes::CMoveOnCylinder::OnDie() -- preserve current camera as old camera and request a single cut.
// Leaf: ?OnDie@CMoveOnCylinder@NCameraModes@@UAEXXZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Identity: proven; own camera mode RTTI virtual slot22 and both PDB OnDie declaration; reads CBase::CameraManager+0x40, copies Camera4 to OldCamera0x3C (56 bytes), sets SingleCut0xB0.
// Layout: no mode/CCamera header. Shared CGameCameraManager header carries donor container offsets: SingleCut+0xBC instead of retail+0xB0 (vector/list differences); local named projection uses actual copy destinations. CBase header is an unrelated namesake; real scoped base fields proven by same-mode methods.
#include "engine/C3DVector.h"
class CRightHandedSet{public:C3DVector Up,Forward;};class CCamera{public:C3DVector WorldPosition;CRightHandedSet RHSet;bool HeightLocked,ViewVecZLocked;unsigned long FOVFlags;float HorizontalFOV,VerticalFOV,Zoom;};
class CGameCameraManager{public:unsigned char _unmodeled00[4];CCamera Camera,OldCamera;unsigned char _unmodeled74[0xb0-0x74];bool SingleCut;};
namespace NCameraModes{class CBase{public:virtual ~CBase(){}unsigned char _unmodeled04[0x40-4];CGameCameraManager* CameraManager;};class CMoveOnCylinder:public CBase{public:virtual void OnDie();};}
void NCameraModes::CMoveOnCylinder::OnDie(){CGameCameraManager& manager=*CameraManager;manager.OldCamera=manager.Camera;manager.SingleCut=true;}