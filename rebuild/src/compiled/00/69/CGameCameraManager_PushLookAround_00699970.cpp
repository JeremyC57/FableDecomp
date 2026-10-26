// CGameCameraManager::PushLookAround() -- start CRotateAround camera mode and play ROTATE_START
// Retail00699970; native baseA4, modeC8; explicit native vtable views from RTTI, no invented virtual slots.
#define CGameCameraManager CGameCameraManagerDonor
#include "engine/CGameCameraManager.h"
#undef CGameCameraManager
#include "fable_string.h"
#define CThing CThingDonor
#include "engine/_quarantine/CThing.h"
#undef CThing
class CThing { public:unsigned char NativeState[0x91];unsigned char LifetimeFlags;};
struct CObservedThingPointer {void* VTable;void* PointerInfo;CThing* GetPItem()const;}; //00A01B50
class CCameraManagerDef;struct CCameraManagerDefPointer {CCameraManagerDef* Object;operator CCameraManagerDef*()const{return Object;}};
struct CCameraModeDefPointer {void* Object;};
extern CCameraModeDefPointer g_DefaultCameraModeDef; //013BA8D8
class CGameCameraManager;
struct CPhysicsState {float Position[3],Velocity[3],Acceleration[3];void* Attractors;void* Incidentals;bool Cull;};
namespace NCameraModes {
class CBase { public:
 void* VTable;float DefaultFOV,DefaultRoll,DefaultOffset;CPhysicsState LookAt;
 CGameCameraManager* Manager;void* ModeDef;void *SubFirst,*SubLast,*SubEnd;
 CPhysicsState GoTo;float FOV,Roll,Offset,RightStick[2],LeftStick[2];bool ViewOccluded;
 CBase(CGameCameraManager*,const CCameraModeDefPointer&); //0084E420
};
extern void* g_RotateAroundVTable[]; //0125C674; RTTI CRotateAround
extern void* g_OrbitGameVTable[]; //0125C6DC; RTTI COrbitGame
struct CRotateAround:CBase {float StringLengthXY,HeightOffset,XAxis[3],YAxis[3],Theta;__forceinline CRotateAround(CGameCameraManager* m,const CCameraModeDefPointer& d):CBase(m,d){VTable=g_RotateAroundVTable;}};
struct COrbitGame:CBase {float OrbitState[6];COrbitGame(CGameCameraManager* m,const CCameraModeDefPointer& d):CBase(m,d){VTable=g_OrbitGameVTable;}};
}
struct CCountedCameraMode {NCameraModes::CBase* Data;void* Info;CCountedCameraMode(NCameraModes::CBase*);}; //0056C880; native owned parameter consumed by697E20
struct CManualScriptModule;
struct CManualScriptVTable {void* ScalarDeletingDestructor;void* InheritedSlot1;bool (__fastcall* AllowsManualControl)(CManualScriptModule*);};
struct CManualScriptModule {CManualScriptVTable* VTable;bool AllowsManualControl(){return VTable->AllowsManualControl(this);}};
void __fastcall PlayCameraSoundThroughObservedThing(CThing*,CCameraManagerDef*,const CCharString&); //00698A30
class CGameCameraManager { public:
 void* VTable;unsigned char CameraAndControlState[0xd0];
 CManualScriptModule* PCurrentScriptModule;void* CurrentScriptInfo;CManualScriptModule* PPendingScriptModule;void* PendingScriptInfo;
 unsigned char ScriptAndQuakeState[0x1c];CObservedThingPointer PObservedThing;
 unsigned char CameraLists[0x24];bool UsingCameraManually,BlockModeChange;unsigned char Alignment[2];void* CameraManagerSet;CCameraManagerDefPointer PDef;
 void SetPendingMode(CCountedCameraMode); //00697E20
 void PushLookAround();
};
void CGameCameraManager::PushLookAround(){
 if(PObservedThing.GetPItem()&&!(PObservedThing.GetPItem()->LifetimeFlags&1)) {
 if(!PPendingScriptModule&&(!PCurrentScriptModule||PCurrentScriptModule->AllowsManualControl())) {
 BlockModeChange=true;UsingCameraManually=true;
 {CCharString sound("ROTATE_START",-1);PlayCameraSoundThroughObservedThing(PObservedThing.GetPItem(),PDef,sound);}
 SetPendingMode(new NCameraModes::CRotateAround(this,g_DefaultCameraModeDef));
 }
 }
}