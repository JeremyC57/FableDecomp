// CGameScriptInterface::GetPreviouslyWieldedRangedWeaponName(CCharString&) -- resolve the previous weapon definition name
// Retail 00897630; actual ROOT fable_string and native inventory offsets; ranged always true.
#include "fable_compiler.h"
#include "engine/CGameScriptInterface.h"
#include "engine/CPlayerManager.h"
#define CPlayer CPlayerPdb
#include "engine/CPlayer.h"
#undef CPlayer
#include "fable_string.h"
struct ComponentEntry {long Type;void* Component;};
class CTCBase;
// Naming (NF437, 2026-11-25): the LowerBound split -- the row-local map stand-in ComponentMap (?LowerBound@ComponentMap@@QBEPAUComponentEntry@@ABJ@Z) replaced by the shared
//   CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<..> > typedef of engine/CVectorMap.h that the landed AI rows use (0x00929F10): CThing::Components at +0x44 (0x10 bytes),
//   so the call binds the established ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@...@@QBE spelling at 0x004365B0.
#include "engine/ETCInterfaceType.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > ComponentMap;
// Native CThing map+44; donor+48. Life flag+91 and inventory19 bit are verified in retail.
struct CTCInventoryWeaponsNative;
struct CThingNative {void* VTable;unsigned char Base[0x1c];unsigned int UsingComponents[9];ComponentMap Components;unsigned char State[0x3D];unsigned char LifeFlags;FABLE_FORCEINLINE bool GetInventory(CTCInventoryWeaponsNative** out);};
class CThingPlayerCreature : public CThingNative { };
class CPlayer : public CPlayerPdb { public: CThingPlayerCreature* GetPControlledCreature() const; }; //00487DC0
struct CTCInventoryWeaponsNative {unsigned char Base[0x168];long PreviouslyWieldedMeleeWeaponDefIndex,PreviouslyWieldedRangedWeaponDefIndex; FABLE_FORCEINLINE long GetPreviousRanged()const{return PreviouslyWieldedRangedWeaponDefIndex;}}; // donor184/188; native168/16c
class CDefString { public:long TablePos;CDefString(){} CDefString(const CDefString& x):TablePos(x.TablePos){}};
class CDefinitionManager { public: CDefString GetDefNameFromGlobalIndex(long) const; }; // 0x009ACCC0 (Ego_r.pdb, const)
class CDefStringTable { public:CCharString GetString(long) const;}; //009D49B0 Ego_r/FableWin ?GetString@CDefStringTable@@QBE?AVCCharString@@J@Z (agentNF16)
extern CDefStringTable GDefStringTable;   // 0x013CA828 ?GDefStringTable@@3VCDefStringTable@@A (ESTABLISHED; naming wave 21) //013CA828
struct CCharStringMethods:CCharString {bool operator!=(const char*)const;}; //0099E960
FABLE_FORCEINLINE bool CThingNative::GetInventory(CTCInventoryWeaponsNative** out){
 if(UsingComponents[0]&0x80000){const ComponentMap* map=&Components;const ComponentMap::value_type* it=map->LowerBound((ETCInterfaceType)19);const ComponentMap::value_type* end=map->end();*out=(CTCInventoryWeaponsNative*)(it==end||it->first>19?end:it)->second;return true;}return false;
}
FABLE_FORCEINLINE long GetPrevious(CTCInventoryWeaponsNative* inventory){return inventory->PreviouslyWieldedRangedWeaponDefIndex;}
struct CGameScriptInterfaceMethods:CGameScriptInterface {bool GetPreviouslyWieldedRangedWeaponName(CCharString&)const;};
bool CGameScriptInterfaceMethods::GetPreviouslyWieldedRangedWeaponName(CCharString& result)const{
 CTCInventoryWeaponsNative* inventory;CThingPlayerCreature* hero=PlayerManager->GetMainPlayer().GetPControlledCreature();
 if(hero && !(hero->LifeFlags&1) && hero->GetInventory(&inventory)) {

 long index=inventory->PreviouslyWieldedRangedWeaponDefIndex;
 if(index>0){{CCharString name=GDefStringTable.GetString(reinterpret_cast<CDefinitionManager*>(DefinitionManager)->GetDefNameFromGlobalIndex(index).TablePos);result=name;}if(static_cast<CCharStringMethods&>(result)!="")return true;}
 } return true;
}