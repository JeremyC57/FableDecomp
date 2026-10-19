// CGameCameraManager::Update() -- update camera state for a live observed thing, retaining current view on a single cut
// Retail0069B790 184bytes. Actual donor manager header retained; native CTools shrinks4, lists shrink4 each.
#define CGameCameraManager CGameCameraManagerDonor
#include "engine/CGameCameraManager.h"
#undef CGameCameraManager
#define CThing CThingDonor
#include "engine/_quarantine/CThing.h"
#undef CThing
struct CThing {unsigned char NativeState[0x91];unsigned char LifetimeFlags;};
struct CObservedThingPointer {void* VTable;void* PointerInfo;CThing* GetPItem()const;}; //00A01B50
struct C3DVector {float X,Y,Z;};
struct CRightHandedSet {C3DVector Up,Forward;};
struct CCamera {C3DVector WorldPosition;CRightHandedSet RHSet;bool HeightLocked,ViewVecZLocked;unsigned int FOVFlags;float HorizontalFOV,VerticalFOV,Zoom;};
struct CCameraTools {unsigned char NativeTools[0x20];void FrameUpdate();}; //00877D20
struct CCameraBase {void FrameUpdate();}; //0084DC10
struct CGameCameraManager {
 void* VTable;CCamera Camera,OldCamera;void* World;CCameraTools CamTools;
 CCameraBase* CurrentMode;void* CurrentInfo;void* PendingMode;void* PendingInfo;
 bool NewAutoMode,ObservedThingInMelee,ObservedThingInBuilding;unsigned char Align;
 void* AvailableCamPoints[1];bool SingleCut;unsigned char ControlsAndScripts[0x4f];CObservedThingPointer PObservedThing;
 void UpdatePrepareFlags(); //0069B6F0
 void UpdateMode(); //0069A2A0
 void PromotePendingScriptAndResetPreloadFlags(); //00697C60
 void UpdateHeightLimits(); //00699670
 void UpdateCreatureControlAngleXY(); //00697760
 void UpdateRightStickLookAround(); //00697CC0
 void UpdateEarthQuakes(); //00698540
 void CameraValidationCheck(); //0069AB80
 void Update();
};
void CGameCameraManager::Update(){
 if(PObservedThing.GetPItem()&&!(PObservedThing.GetPItem()->LifetimeFlags&1)) {
  UpdatePrepareFlags();UpdateMode();PromotePendingScriptAndResetPreloadFlags();OldCamera=Camera;
  UpdateHeightLimits();CamTools.FrameUpdate();CurrentMode->FrameUpdate();
  UpdateCreatureControlAngleXY();UpdateRightStickLookAround();UpdateEarthQuakes();CameraValidationCheck();
  if(SingleCut){OldCamera=Camera;SingleCut=false;}
 }else OldCamera=Camera;
}