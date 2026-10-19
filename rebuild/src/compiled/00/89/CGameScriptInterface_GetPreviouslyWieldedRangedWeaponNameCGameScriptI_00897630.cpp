// CGameScriptInterface::GetPreviouslyWieldedRangedWeaponName(CCharString&) -- resolve the previous weapon definition name
// Retail 00897630; actual ROOT fable_string and native inventory offsets; ranged always true.
#include "engine/CGameScriptInterface.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "fable_string.h"
struct ComponentEntry {long Type;void* Component;};
struct ComponentMap {ComponentEntry *Start,*Finish;ComponentEntry* LowerBound(const long&);}; // 004365B0 names,0040F020 other uses: identical body
// Native CThing map+44; donor+48. Life flag+91 and inventory19 bit are verified in retail.
struct CTCInventoryWeaponsNative;
struct CThingNative {void* VTable;unsigned char Base[0x1c];unsigned int UsingComponents[9];ComponentMap Components;unsigned char State[0x45];unsigned char LifeFlags;__forceinline bool GetInventory(CTCInventoryWeaponsNative** out);};
struct CPlayerMethods:CPlayer {CThingNative* GetHeroThing();}; //00487DC0
struct CPlayerManagerMethods:CPlayerManager {CPlayerMethods* GetMainPlayer();}; //00449970
struct CTCInventoryWeaponsNative {unsigned char Base[0x168];long PreviouslyWieldedMeleeWeaponDefIndex,PreviouslyWieldedRangedWeaponDefIndex; __forceinline long GetPreviousRanged()const{return PreviouslyWieldedRangedWeaponDefIndex;}}; // donor184/188; native168/16c
struct CDefString {long TablePos;CDefString(){} CDefString(const CDefString& x):TablePos(x.TablePos){}};
struct CDefinitionManagerMethods {CDefString GetDefNameFromGlobalIndex(long);}; //009ACCC0
struct CDefStringTable {CCharString GetString(long);}; //009D49B0
extern CDefStringTable g_DefStringTable; //013CA828
struct CCharStringMethods:CCharString {bool operator!=(const char*)const;}; //0099E960
__forceinline bool CThingNative::GetInventory(CTCInventoryWeaponsNative** out){
 if(UsingComponents[0]&0x80000){ComponentMap* map=&Components;ComponentEntry* it=map->LowerBound(19);ComponentEntry* end=map->Finish;*out=static_cast<CTCInventoryWeaponsNative*>((it==end||it->Type>19?end:it)->Component);return true;}return false;
}
__forceinline long GetPrevious(CTCInventoryWeaponsNative* inventory){return inventory->PreviouslyWieldedRangedWeaponDefIndex;}
struct CGameScriptInterfaceMethods:CGameScriptInterface {bool GetPreviouslyWieldedRangedWeaponName(CCharString&)const;};
bool CGameScriptInterfaceMethods::GetPreviouslyWieldedRangedWeaponName(CCharString& result)const{
 CTCInventoryWeaponsNative* inventory;CThingNative* hero=static_cast<CPlayerManagerMethods*>(PlayerManager)->GetMainPlayer()->GetHeroThing();
 if(hero && !(hero->LifeFlags&1) && hero->GetInventory(&inventory)) {

 long index=inventory->PreviouslyWieldedRangedWeaponDefIndex;
 if(index>0){{CCharString name=g_DefStringTable.GetString(reinterpret_cast<CDefinitionManagerMethods*>(DefinitionManager)->GetDefNameFromGlobalIndex(index).TablePos);result=name;}if(static_cast<CCharStringMethods&>(result)!="")return true;}
 } return true;
}