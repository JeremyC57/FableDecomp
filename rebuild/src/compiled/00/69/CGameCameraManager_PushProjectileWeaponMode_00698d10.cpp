// CGameCameraManager::PushProjectileWeaponMode -- build projectile camera at the current targeting FOV.
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
#include "engine/CTCHeroRangedAttacks.h"
#include <new>
struct CCameraNativeDef:CIVCountedPointeeBase {void ReleaseReference(){--IVRefCount;if(!IVRefCount){typedef void(CCameraNativeDef::*F)()const;(this->*static_cast<const F*>(__vftable)[1])();}}};
class CCameraModeDef:public CCameraNativeDef { public:unsigned char BeforeMode[0x34];long CameraMode;}; //native3C vs donor40
class CCameraManagerDef { public:unsigned char BeforeModes[0x54];int InitialMode,ZTargetMode,ProjectileWeaponMode;}; //native58/5C vs donor64/68
struct CCameraDefPointer {CCameraModeDef* Object;CCameraDefPointer():Object(0){}CCameraDefPointer(const CCameraDefPointer& p):Object(p.Object){if(Object)++Object->IVRefCount;}~CCameraDefPointer(){if(Object)Object->ReleaseReference();}};
// Quarantined manager header internally contradicts sizeof1/vptr4; declaration-only view.
class CGameDefinitionManager { public:bool GetDef(long,CCameraDefPointer*);}; //56D0C0
struct CCameraMode {void* Vtable;};
struct CCameraModeOwner {unsigned References;void(__fastcall* Destroy)(void*);void* Data;};
struct CCameraModePointer {CCameraMode* Data;CCameraModeOwner* Info;CCameraModePointer(CCameraMode*);CCameraModePointer(const CCameraModePointer&);~CCameraModePointer();}; //56C050
struct CCameraScript {void* Vtable;bool IsFinished()const{typedef bool(CCameraScript::*F)()const;return(this->*static_cast<const F*>(Vtable)[2])();}};
struct CCameraScriptPointer {CCameraScript* Data;CCameraModeOwner* Info;};
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
typedef std::pair<ETCInterfaceType, CTCBase*> CCameraComponentEntry;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:CCameraComponentEntry* Begin;CCameraComponentEntry* End;CCameraComponentEntry* Capacity;const CCameraComponentEntry* LowerBound(const ETCInterfaceType& key) const;
 __forceinline CTCHeroRangedAttacks* Find()const{ETCInterfaceType key = (ETCInterfaceType)(0x81);const CCameraComponentEntry* p=LowerBound(key);if(p==End||0x81<p->first)p=End;return ((CTCHeroRangedAttacks*)p->second);}};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CCameraComponentMap;
class CThing { public:unsigned char Prefix[0x30];unsigned char RangedFlag;unsigned char ToMap[0x13];CCameraComponentMap Components;unsigned char BeforeDeleted[0x41];unsigned char Deleted;};
struct CCameraRangedMethods:CTCHeroRangedAttacks {float GetProjectileWeaponFirstPersonTargetingModeZoomFOV()const;}; //789CC0
struct CCameraObservedPointer {void* Vtable;void* Info;const CThing* GetPItem()const;}; //A01B50
struct CCameraDefArray {CCameraDefPointer* Begin;CCameraDefPointer* End;CCameraDefPointer* Capacity;};
void __fastcall PlayCameraSoundThroughObservedThing(const CThing*,const CCameraManagerDef&,const CCharString&); //698A30
class CGameCameraManager { public:
 CGameCameraManager(const CWorld&); //PDB13517 donor6C1E60
 unsigned char Prefix[0x74];CWorld* World;unsigned char CamTools[0x20];CCameraModePointer CurrentMode,PendingMode;
 unsigned char BeforeScripts[0x2c];CCameraScriptPointer CurrentScript,PendingScript;unsigned char BeforeObserved[0x1c];CCameraObservedPointer Observed;
 CCameraDefArray CameraList;CCameraDefPointer* CameraIndex;CCameraDefPointer* InitialIndex;CCameraDefArray SpecialList;CCameraDefPointer* SpecialIndex;
 bool UsingCameraManually,BlockModeChange;unsigned char Pad[2];void* ManagerSet;CCameraManagerDef* PDef;
 void PreparePendingMode(CCameraDefPointer); //698840
 void SetCameraMode(CCameraModePointer); //697E20
 void PushProjectileWeaponMode();
};
#ifdef _MSC_VER
typedef char NativeObserved100[offsetof(CGameCameraManager,Observed)==0x100?1:-1];
typedef char NativeDef134[offsetof(CGameCameraManager,PDef)==0x134?1:-1];
#endif

namespace NCameraModes {class CProjectileWeapon:public CCameraMode { public:unsigned char State[0x104];CProjectileWeapon(CGameCameraManager&,const CCameraDefPointer&,float,float);};}
#ifdef _MSC_VER
typedef char NativeProjectile108[sizeof(NCameraModes::CProjectileWeapon)==0x108?1:-1];
#endif
void CGameCameraManager::PushProjectileWeaponMode(){
 BlockModeChange=true;
 if(Observed.GetPItem()&&!(Observed.GetPItem()->Deleted&1)&&!PendingScript.Data&&(!CurrentScript.Data||CurrentScript.Data->IsFinished())){
 CCameraDefPointer mode;World->DefinitionManager->GetDef(PDef->ProjectileWeaponMode,&mode);
 const CThing* thing=Observed.GetPItem();const CCameraRangedMethods* ranged;
 // Retail leaves the lookup output untouched if component81 is absent;
 // callers must supply an observed ranged-attack creature.
 if(thing->RangedFlag&2)ranged=static_cast<const CCameraRangedMethods*>(thing->Components.Find());
 float zoom=ranged->GetProjectileWeaponFirstPersonTargetingModeZoomFOV();
 SetCameraMode(CCameraModePointer(new NCameraModes::CProjectileWeapon(*this,mode,zoom,zoom)));
 }
}