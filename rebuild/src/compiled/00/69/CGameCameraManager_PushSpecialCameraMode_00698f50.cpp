// CGameCameraManager::PushSpecialCameraMode -- cycle the configured special camera definitions.
#define CGameCameraManager CGameCameraManagerDonor
#define CCameraManagerDef CCameraManagerDefDonor
#define CCameraModeDef CCameraModeDefDonor
#define CThing CThingDonor
#include "engine/CGameCameraManager.h"
#include "engine/_quarantine/CCameraManagerDef.h"
#include "engine/_quarantine/CCameraModeDef.h"
#include "engine/_quarantine/CThing.h"
#undef CGameCameraManager
#undef CCameraManagerDef
#undef CCameraModeDef
#undef CThing
#include "engine/CWorld.h"
#include "engine/CIVCountedPointeeBase.h"
#include "fable_string.h"
struct CCameraNativeDef:CIVCountedPointeeBase {void ReleaseReference(){--IVRefCount;if(!IVRefCount){typedef void(CCameraNativeDef::*F)()const;(this->*static_cast<const F*>(__vftable)[1])();}}};
struct CCameraModeDef:CCameraNativeDef {unsigned char BeforeMode[0x34];long CameraMode;}; //native3C vs donor40
struct CCameraManagerDef {unsigned char BeforeModes[0x54];int InitialMode,ZTargetMode,ProjectileWeaponMode;}; //native58/5C vs donor64/68
struct CCameraDefPointer {CCameraModeDef* Object;CCameraDefPointer():Object(0){}CCameraDefPointer(const CCameraDefPointer& p):Object(p.Object){if(Object)++Object->IVRefCount;}~CCameraDefPointer(){if(Object)Object->ReleaseReference();}};
// Quarantined manager header internally contradicts sizeof1/vptr4; declaration-only view.
struct CGameDefinitionManager {bool GetDef(long,CCameraDefPointer*);}; //56D0C0
struct CCameraMode {void* Vtable;};
struct CCameraModeOwner {unsigned References;void(__fastcall* Destroy)(void*);void* Data;};
struct CCameraModePointer {CCameraMode* Data;CCameraModeOwner* Info;CCameraModePointer();/*PDB fablewin55953*/CCameraModePointer(const CCameraModePointer&);~CCameraModePointer();void Reset(CCameraMode*);}; //56C050
struct CCameraScript {void* Vtable;bool IsFinished()const{typedef bool(CCameraScript::*F)()const;return(this->*static_cast<const F*>(Vtable)[2])();}};
struct CCameraScriptPointer {CCameraScript* Data;CCameraModeOwner* Info;};
struct CThing {unsigned char BeforeDeleted[0x91];unsigned char Deleted;};
struct CCameraObservedPointer {void* Vtable;void* Info;const CThing* GetPItem()const;}; //A01B50
struct CCameraDefArray {CCameraDefPointer* Begin;CCameraDefPointer* End;CCameraDefPointer* Capacity;};
void __fastcall PlayCameraSoundThroughObservedThing(const CThing*,const CCameraManagerDef&,const CCharString&); //698A30
struct CGameCameraManager;
struct CCameraBaseInit {CCameraMode* AllocMain(CGameCameraManager&,const CCameraDefPointer&)const;}; //84D270; PDB20801
struct CCameraInitPointer {CCameraBaseInit* Data;CCameraModeOwner* Info;CCameraBaseInit* operator->()const{return Data;}};
struct CCameraRegisteredValue {int Key;CCameraInitPointer Second;};
struct CCameraRegisteredEntry {unsigned Color;CCameraRegisteredEntry* Parent;CCameraRegisteredEntry* Left;CCameraRegisteredEntry* Right;CCameraRegisteredValue Value;};
// Actual STL tree iterator copy constructor: fablewin02206D80; destructor021F65B0.
// Native one-pointer projection excludes donor iterator debugging machinery.
struct CCameraRegisteredIterator {CCameraRegisteredEntry* Node;explicit CCameraRegisteredIterator(CCameraRegisteredEntry* p):Node(p){}CCameraRegisteredIterator(const CCameraRegisteredIterator& x):Node(x.Node){}~CCameraRegisteredIterator(){}CCameraRegisteredValue* operator->()const{return &Node->Value;}};
namespace NCameraModes {enum EMode {}; } //real map key enum; mode definition stores long
struct CCameraRegisteredMap {unsigned char State[12];CCameraRegisteredIterator Find(const NCameraModes::EMode&);}; //69CF90
struct CGameCameraManager {
 CGameCameraManager(const CWorld&); //actual PDB13517
 unsigned char Prefix[0x74];CWorld* World;unsigned char CamTools[0x20];CCameraModePointer CurrentMode,PendingMode;
 unsigned char BeforeScripts[0x2c];CCameraScriptPointer CurrentScript,PendingScript;unsigned char BeforeObserved[0x1c];CCameraObservedPointer Observed;
 CCameraDefArray CameraList;CCameraDefPointer* CameraIndex;CCameraDefPointer* InitialIndex;CCameraDefArray SpecialList;CCameraDefPointer* SpecialIndex;
 bool UsingCameraManually,BlockModeChange;unsigned char Pad[2];void* ManagerSet;CCameraManagerDef* PDef;CCameraRegisteredMap RegisteredModes;
 void PreparePendingMode(CCameraDefPointer); //698840
 void SetCameraMode(CCameraModePointer); //697E20
 void PushSpecialCameraMode();
};
#ifdef _MSC_VER
typedef char NativeObserved100[offsetof(CGameCameraManager,Observed)==0x100?1:-1];
typedef char NativeRegistered138[offsetof(CGameCameraManager,RegisteredModes)==0x138?1:-1];
typedef char NativeEntryData14[offsetof(CCameraRegisteredEntry,Value)+offsetof(CCameraRegisteredValue,Second)==0x14?1:-1];
typedef char NativeDef134[offsetof(CGameCameraManager,PDef)==0x134?1:-1];
#endif
void CGameCameraManager::PushSpecialCameraMode(){
 BlockModeChange=false;UsingCameraManually=true;
 if(Observed.GetPItem()&&!(Observed.GetPItem()->Deleted&1)&&!PendingScript.Data&&(!CurrentScript.Data||CurrentScript.Data->IsFinished())){
 {CCharString sound("PUSH_SPECIALITY_CAMERA_MODE",-1);const CCameraManagerDef& def=*PDef;PlayCameraSoundThroughObservedThing(Observed.GetPItem(),def,sound);}
 CCameraDefPointer mode=*SpecialIndex++;
 if(SpecialIndex==SpecialList.End)SpecialIndex=SpecialList.Begin;
 CCameraRegisteredIterator entry=RegisteredModes.Find(static_cast<NCameraModes::EMode>(mode.Object->CameraMode));
 PendingMode.Reset(entry->Second->AllocMain(*this,mode));
 }
}