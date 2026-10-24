// CGameCameraManager::PushUpManualCameraList() -- the player steps the manual camera list up: when the observed thing is alive and
// no script camera module is pending or blocking, move the list index back one mode and prepare it (sound CAMERA_LIST_PUSH_UP),
// or play CAMERA_LIST_PUSH_ERROR at the top of the list
// Leaf: PushUpManualCameraList
// Retail: Fable.exe (TLC, VC7.1) @ 0x00698B50, 218 bytes (+ int3 padding to 224), `ret`.
// Identity: the near-twin of the landed PushDownManualCameraList 0x00698C30 (retail differs only in the list-end compare
// `[esi+108h]` = CameraListFirst, `add eax,-4` and the PUSH_UP literal); the landed model ported (retry12). The parked hand193
// model (a local CGameCameraManager model with CIntelligentPointer / CDefPointer templates) fetched the observed thing after
// each string ctor; the twin's donor-header model reads GetPItem at the call as retail does.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, the script module's virtual slot 2, CCharString ctor 0x0099EBF0 /
// dtor 0x0099EAE0, PreparePendingMode(CDefPointer<const CCameraModeDef>) 0x00698840, the `__fastcall`
// PlayCameraSoundThroughObservedThing(CThing*, const CCameraManagerDef*, const CCharString&) 0x00698A30.
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
struct CThing {unsigned char NativeState[0x91];unsigned char LifetimeFlags;}; // native bit0 means unavailable; all GetPItem guards use this mask
struct CObservedThingPointer {void* VTable;void* PointerInfo;CThing* GetPItem()const;}; // A01B50 reads info+0 and may release stale info
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
struct CCameraManagerDef;
struct CCameraManagerDefPointer {CCameraManagerDef* Object;operator CCameraManagerDef*()const{return Object;}};
void __fastcall PlayCameraSoundThroughObservedThing(CThing*,CCameraManagerDef*,const CCharString&); //00698A30
struct CGameCameraManager {
 void* VTable;unsigned char CameraAndControlState[0xd0];
 CManualScriptModule* PCurrentScriptModule;void* CurrentScriptInfo;CManualScriptModule* PPendingScriptModule;void* PendingScriptInfo;
 unsigned char ScriptAndQuakeState[0x1c];CObservedThingPointer PObservedThing;
 CCameraDefPointer *CameraListFirst,*CameraListLast,*CameraListEnd,*CameraListIndex;
 CCameraDefPointer *InitialModeCameraListIndex,*SpecialFirst,*SpecialLast,*SpecialEnd,*SpecialIndex;
 bool UsingCameraManually,BlockModeChange;unsigned char Alignment[2];void* CameraManagerSet;CCameraManagerDefPointer PDef;
 CCameraDefPointer* CameraListBegin()const{return CameraListFirst;}
 void PreparePendingMode(CCameraDefPointer); //00698840 consumes its byvalue arg
 void PushUpManualCameraList();
};
void CGameCameraManager::PushUpManualCameraList(){
 BlockModeChange=false;UsingCameraManually=true;
 if(PObservedThing.GetPItem()&&!(PObservedThing.GetPItem()->LifetimeFlags&1)) {
  if(!PPendingScriptModule) {
   if(!PCurrentScriptModule||PCurrentScriptModule->AllowsManualControl()) {
    if(CameraListIndex==CameraListBegin()) {
     CCharString sound("CAMERA_LIST_PUSH_ERROR",-1);
     PlayCameraSoundThroughObservedThing(PObservedThing.GetPItem(),PDef,sound);
    }else {
     --CameraListIndex;
     PreparePendingMode(*CameraListIndex);
     CCharString sound("CAMERA_LIST_PUSH_UP",-1);
     PlayCameraSoundThroughObservedThing(PObservedThing.GetPItem(),PDef,sound);
    }
   }
  }
 }
}