// 005cbc36 CTCInventoryWeapons::OnItemAdded
// Leaf: ?OnItemAdded@CTCInventoryWeapons@@MAEXJ_N0@Z
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Flags: /O1 /Oy-
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"
#include "engine/CDefClassBase.h"
#define CDefinitionManager CDefinitionManagerFields
#include "engine/CDefinitionManager.h"
#undef CDefinitionManager
#define CTCBase CTCBaseFields
#include "engine/CTCBase.h"
#undef CTCBase
#define CTCInventoryBase CTCInventoryBaseFields
#include "engine/_quarantine/CTCInventoryBase.h"
#undef CTCInventoryBase
#define CWeaponDef CWeaponDefFields
#include "engine/_quarantine/CWeaponDef.h"
#undef CWeaponDef
#include "engine/_quarantine/CThingBaseDef.h"
#define CTCWeapon CTCWeaponFields
#include "engine/CTCWeapon.h"
#undef CTCWeapon
#include "engine/CBaseClassNonCopyable.h"
class CCharStringData;class CCharString{protected:CCharStringData*PStringData;public:CCharString(const char*,long=-1);~CCharString();};
class CWeaponDef:public CWeaponDefFields{public:static CCharString __fastcall StatGetTypeName(){return CCharString("CWeaponDef");}};
enum EWeaponClass{};namespace NUISystem{class CComponent;}
class CTCWeapon:public CTCWeaponFields{public:static bool __fastcall IsMeleeWeaponClass(EWeaponClass);static bool __fastcall IsProjectileWeaponClass(EWeaponClass);};
class CDefinitionManager:public CDefinitionManagerFields{public:
 CDefPointer<const CDefClassBase>GetPDefFromGlobalIndex(long)const;
 CDefPointer<CDefClassBase>GetPDefFromGlobalIndex(long);
 template<class T>bool GetDef(long,CDefPointer<const T>*)const;
};
class CGameDefinitionManager:public CDefinitionManager{};
// Layout: shared CTCBase flattened vptr restored as original real base, reference4 and flags8/9.
class CTCBase:public CBaseClassNonCopyable{protected:CThing&ParentThing;private:bool ToBeRemoved,IsInGlobalUpdateTCs;protected:const CGameDefinitionManager&GetDefinitionManager()const;};
namespace NInventory{
// Layout: shared base0x130 imported; original polymorphic CTCBase prefix0xc plus unread storage.
class CTCInventoryBase:public CTCBase{unsigned char _pad_0xc[0x130-0xc];protected:virtual void OnItemAdded(long,bool,bool);};
}
class CBaseObject;class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
template<class T>bool __fastcall IS_THING_ALIVE(const T&);
// Layout: original second base occupies130..133; packed intelligent-pointer members at134 and141.
// Byte storage preserves original field offsets without adding a packing directive.
class CTCInventoryWeapons:public NInventory::CTCInventoryBase{
 unsigned char _pad_0x130[4];unsigned char PActiveMeleeWeapon[8];unsigned char ActiveMeleeWeaponDefIndex[4];bool MeleeWeaponCarriedOnLoad;unsigned char PActiveRangedWeapon[8];
 public:void SetThingAsActiveWeapon(long,NUISystem::CComponent*);void RefreshSheathedWeapons();
 protected:virtual void OnItemAdded(long,bool,bool);
};
template<class T>__declspec(noinline)bool CDefinitionManager::GetDef(long index,CDefPointer<const T>*out)const{
 if(index<=0)return false;CDefPointer<const CDefClassBase>value=GetPDefFromGlobalIndex(index);if(value.IsNull())return false;out->SetDynamicCast(value);return true;
}
template<class T>__declspec(noinline)bool CDefClassBase::GetSubDef(CDefPointer<const T>*out)const{
 const CSubDefInfo*info=GetPSubDefInfo(T::StatGetTypeName());if(!info)return false;CDefPointer<const T>value(PDefManager->GetPDefFromGlobalIndex(info->DefIndex));out->SetDynamicCast(value);return true;
}
void CTCInventoryWeapons::OnItemAdded(long itemType,bool a,bool b){
 NInventory::CTCInventoryBase::OnItemAdded(itemType,a,b);
 CDefPointer<const CThingBaseDef>def;GetDefinitionManager().GetDef(itemType,&def);
 CDefPointer<const CWeaponDef>weaponDef;
 bool found=reinterpret_cast<const CDefClassBase*>(def.InternalGetObject())->GetSubDef(&weaponDef);
 const CWeaponDef*weapon=reinterpret_cast<const CWeaponDef*>(weaponDef.InternalGetObject());
 if(found){
 EWeaponClass weaponClass=*reinterpret_cast<const EWeaponClass*>(weapon->Class);
 if(CTCWeapon::IsMeleeWeaponClass(weaponClass)&&!IS_THING_ALIVE(reinterpret_cast<const CIntelligentPointer<CThing>&>(PActiveMeleeWeapon)))SetThingAsActiveWeapon(itemType,0);
 else if(CTCWeapon::IsProjectileWeaponClass(*reinterpret_cast<const EWeaponClass*>(weapon->Class))&&!IS_THING_ALIVE(reinterpret_cast<const CIntelligentPointer<CThing>&>(PActiveRangedWeapon)))SetThingAsActiveWeapon(itemType,0);
 RefreshSheathedWeapons();
 }
}