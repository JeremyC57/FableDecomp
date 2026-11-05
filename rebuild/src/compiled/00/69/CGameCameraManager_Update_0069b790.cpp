// CGameCameraManager::Update() -- update camera state for a live observed thing, retaining current view on a single cut
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail0069B790 184bytes. Actual donor manager header retained; native CTools shrinks4, lists shrink4 each.
#define CGameCameraManager CGameCameraManagerDonor
#include "engine/CGameCameraManager.h"
#undef CGameCameraManager
#define CThing CThingDonor
#include "engine/_quarantine/CThing.h"
#undef CThing
class CThing { public:unsigned char NativeState[0x91];unsigned char LifetimeFlags;};
class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct CObservedThingPointer {void* VTable;void* PointerInfo;}; //00A01B50
#include "engine/C3DVector.h"
class CRightHandedSet { public:C3DVector Up,Forward;};
class CCamera { public:C3DVector WorldPosition;CRightHandedSet RHSet;bool HeightLocked,ViewVecZLocked;unsigned int FOVFlags;float HorizontalFOV,VerticalFOV,Zoom;};
struct CCameraTools {unsigned char NativeTools[0x20];void FrameUpdate();}; //00877D20
struct CCameraBase {void FrameUpdate();}; //0084DC10
class CGameCameraManager { public:
 void* VTable;CCamera Camera,OldCamera;void* World;CCameraTools CamTools;
 CCameraBase* CurrentMode;void* CurrentInfo;void* PendingMode;void* PendingInfo;
 bool NewAutoMode,ObservedThingInMelee,ObservedThingInBuilding;unsigned char Align;
 void* AvailableCamPoints[1];bool SingleCut;unsigned char ControlsAndScripts[0x4f];CObservedThingPointer PObservedThing;
 void UpdatePrepareFlags(); //0069B6F0
 void UpdateMode(); //0069A2A0
 void PromotePendingScriptAndResetPreloadFlags(); //00697C60
 void UpdateHeightLimits(); //00699670
 void UpdateCreatureControlAngleXY(); //00697760
private:
 void UpdateRightStickLookAround(); //00697CC0
public:
 void UpdateEarthQuakes(); //00698540
 void CameraValidationCheck(); //0069AB80
 void Update();
};
void CGameCameraManager::Update(){
 if(((CThing*)((CBaseIntelligentPointer*)&PObservedThing)->GetPItem())&&!(((CThing*)((CBaseIntelligentPointer*)&PObservedThing)->GetPItem())->LifetimeFlags&1)) {
  UpdatePrepareFlags();UpdateMode();PromotePendingScriptAndResetPreloadFlags();OldCamera=Camera;
  UpdateHeightLimits();CamTools.FrameUpdate();CurrentMode->FrameUpdate();
  UpdateCreatureControlAngleXY();UpdateRightStickLookAround();UpdateEarthQuakes();CameraValidationCheck();
  if(SingleCut){OldCamera=Camera;SingleCut=false;}
 }else OldCamera=Camera;
}