// CGameCameraManager::PushDownManualCameraList() -- select next manual camera and play success/error cue
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail00698c30 218bytes. Real manager/string headers retained; native manager after STL shrink: observed100,list108,index114,using12c,def134.
// Callees GetPItem A01B50, script slot2 (5ED0E0 default false), PreparePendingMode698840, string99EBF0/99EAE0, sound fastcall698A30.
#define CGameCameraManager CGameCameraManagerDonor
#include "engine/CGameCameraManager.h"
#undef CGameCameraManager
#include "fable_string.h"
// Donor layouts are evidence only: native CThing status byte is91 (donorA1), and native def prefix keeps refcount4.
#define CThing CThingDonor
#include "engine/_quarantine/CThing.h"
#undef CThing
#define CCameraModeDef CCameraModeDefDonor
#include "engine/_quarantine/CCameraModeDef.h"
#undef CCameraModeDef
class CThing { public:unsigned char NativeState[0x91];unsigned char LifetimeFlags;}; // native bit0 means unavailable; all GetPItem guards use this mask
class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
struct CObservedThingPointer {void* VTable;void* PointerInfo;}; // A01B50 reads info+0 and may release stale info
struct CCameraModeDefNative;
struct CCameraModeVTable {void* Destructor;void (__fastcall* Destroy)(CCameraModeDefNative*);};
struct CCameraModeDefNative {CCameraModeVTable* VTable;long References;};
struct CCameraDefPointer {
 CCameraModeDefNative* Object;
 CCameraDefPointer():Object(0){}
 CCameraDefPointer(const CCameraDefPointer& p):Object(p.Object){if(Object)++Object->References;}
 ~CCameraDefPointer(){if(Object&&!--Object->References)Object->VTable->Destroy(Object);}
};
struct CManualScriptModule;
struct CManualScriptVTable {void* ScalarDeletingDestructor;void* InheritedSlot1;bool (__fastcall* AllowsManualControl)(CManualScriptModule*);};
struct CManualScriptModule {CManualScriptVTable* VTable;bool AllowsManualControl(){return VTable->AllowsManualControl(this);}};
class CCameraManagerDef;
struct CCameraManagerDefPointer {CCameraManagerDef* Object;operator CCameraManagerDef*()const{return Object;}};
void __fastcall PlayCameraSoundThroughObservedThing(CThing*,CCameraManagerDef*,const CCharString&); //00698A30
class CGameCameraManager { public:
 void* VTable;unsigned char CameraAndControlState[0xd0];
 CManualScriptModule* PCurrentScriptModule;void* CurrentScriptInfo;CManualScriptModule* PPendingScriptModule;void* PendingScriptInfo;
 unsigned char ScriptAndQuakeState[0x1c];CObservedThingPointer PObservedThing;
 CCameraDefPointer *CameraListFirst,*CameraListLast,*CameraListEnd,*CameraListIndex;
 CCameraDefPointer *InitialModeCameraListIndex,*SpecialFirst,*SpecialLast,*SpecialEnd,*SpecialIndex;
 bool UsingCameraManually,BlockModeChange;unsigned char Alignment[2];void* CameraManagerSet;CCameraManagerDefPointer PDef;
 void PreparePendingMode(CCameraDefPointer); //00698840 consumes its byvalue arg
 void PushDownManualCameraList();
};
void CGameCameraManager::PushDownManualCameraList(){
 BlockModeChange=false;UsingCameraManually=true;
 if(((CThing*)((CBaseIntelligentPointer*)&PObservedThing)->GetPItem())&&!(((CThing*)((CBaseIntelligentPointer*)&PObservedThing)->GetPItem())->LifetimeFlags&1)) {
  if(!PPendingScriptModule) {
   if(!PCurrentScriptModule||PCurrentScriptModule->AllowsManualControl()) {
    if(CameraListIndex+1==CameraListLast) {
     CCharString sound("CAMERA_LIST_PUSH_ERROR",-1);
     PlayCameraSoundThroughObservedThing(((CThing*)((CBaseIntelligentPointer*)&PObservedThing)->GetPItem()),PDef,sound);
    }else {
     ++CameraListIndex;
     PreparePendingMode(*CameraListIndex);
     CCharString sound("CAMERA_LIST_PUSH_DOWN",-1);
     PlayCameraSoundThroughObservedThing(((CThing*)((CBaseIntelligentPointer*)&PObservedThing)->GetPItem()),PDef,sound);
    }
   }
  }
 }
}