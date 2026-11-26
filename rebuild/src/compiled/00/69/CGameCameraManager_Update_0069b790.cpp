// CGameCameraManager::Update() -- update camera state for a live observed thing, retaining current view on a single cut
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail0069B790 184bytes. Actual donor manager header retained; native CTools shrinks4, lists shrink4 each.
// Naming (NF157, 2026-11-20; PRIORITY q3987): the local stand-in `struct CCameraBase { void FrameUpdate(); }` removed; CurrentMode (+0x98, the
//   CCountedPointer<NCameraModes::CBase> data pointer) now points at the shared engine/NCameraModes_CBase.h class, whose NF157 opt-in
//   FABLE_RETAIL_NCAMERAMODES_CBASE_FRAMEUPDATE declares Ego_r.pdb 0x008459B0 / FableWin.pdb 0x02883CC0 ?FrameUpdate@CBase@NCameraModes@@QAEXXZ
//   (0x0084DC10; was ?FrameUpdate@CCameraBase@@QAEXXZ). In place: the row's local CGameCameraManager model keeps its other members.
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
#define FABLE_RETAIL_NCAMERAMODES_CBASE_FRAMEUPDATE
#include "engine/NCameraModes_CBase.h"
class CGameCameraManager { public:
 void* VTable;CCamera Camera,OldCamera;void* World;CCameraTools CamTools;
 NCameraModes::CBase* CurrentMode;void* CurrentInfo;void* PendingMode;void* PendingInfo;
 bool NewAutoMode,ObservedThingInMelee,ObservedThingInBuilding;unsigned char Align;
 void* AvailableCamPoints[1];bool SingleCut;unsigned char ControlsAndScripts[0x4f];CObservedThingPointer PObservedThing;
 private:
 void UpdatePrepareFlags();
 public:
 //0069B6F0
 void UpdateMode(); //0069A2A0
private:
 void UpdateCleanupFlags(); //00697C60 ?UpdateCleanupFlags@CGameCameraManager@@AAEXXZ (Ego_r / FableWin, private; NF93 q11378)
public:
 private:
 void UpdateHeightLimits();
 public:
 //00699670
 private:
 void UpdateCreatureControlAngleXY();
 public:
 //00697760
private:
 void UpdateRightStickLookAround(); //00697CC0
public:
 private: void UpdateEarthquake(); public: //00698540; NF296: PDB name, was UpdateEarthQuakes
 void CameraValidationCheck(); //0069AB80
 void Update();
};
void CGameCameraManager::Update(){
 if(((CThing*)((CBaseIntelligentPointer*)&PObservedThing)->GetPItem())&&!(((CThing*)((CBaseIntelligentPointer*)&PObservedThing)->GetPItem())->LifetimeFlags&1)) {
  UpdatePrepareFlags();UpdateMode();UpdateCleanupFlags();OldCamera=Camera;
  UpdateHeightLimits();CamTools.FrameUpdate();CurrentMode->FrameUpdate();
  UpdateCreatureControlAngleXY();UpdateRightStickLookAround();UpdateEarthquake();CameraValidationCheck();
  if(SingleCut){OldCamera=Camera;SingleCut=false;}
 }else OldCamera=Camera;
}