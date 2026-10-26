// CGameScriptInterface::ReturnAllConfiscatedItemsToHero -- restore present hero inventories
// Retail00898940,271 bytes.
#include "engine/CGameScriptInterface.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CTCHeroStats.h"
#include "fable_string.h"
#include "engine/CTCInventory.h"
struct ComponentEntry{long Type;void* Component;};struct ComponentMap{ComponentEntry *Start,*Finish,*Capacity;ComponentEntry* LowerBound(const long&);}; //0040F020
struct CThingNative{void* VTable;unsigned char Base[0x1c];unsigned int UsingComponents[9];ComponentMap Components;unsigned char Rest[0x41];unsigned char DeletedFlags;};
struct PlayerMethods:CPlayer{CThingNative* GetHeroThing();}; //00487DC0
struct PlayerManagerMethods:CPlayerManager{PlayerMethods* GetMainPlayer();}; //00449970
// Native inventory slot73 (+0x124): ReturnAllConfiscatedItems, base005BF91A.
struct InventoryMethods:CTCInventory{typedef void(InventoryMethods::*ReturnMethod)();void ReturnAll(){(this->*static_cast<ReturnMethod*>(__vftable)[73])();}};
FABLE_STATIC_ASSERT(sizeof(InventoryMethods::ReturnMethod)==4);
struct HeroStatsMethods:CTCHeroStats{void SetMoneyAsConfiscated(bool);}; //0057C999
class CGameDefinitionManager{ public:static CGameDefinitionManager& __fastcall Get();long GetDefGlobalIndexFromName(const CCharString&);}; //0044C6B0,009AD410
template<int Key,class T> __forceinline T* GetComponent(CThingNative* thing){T* result;if(thing->UsingComponents[0]&(1u<<Key)){ComponentMap* map=&thing->Components;ComponentEntry* it=map->LowerBound(Key);ComponentEntry* end=map->Finish;result=static_cast<T*>((it==end||it->Type>Key?end:it)->Component);}return result;}
struct CGameScriptInterfaceMethods:CGameScriptInterface{void ReturnAllConfiscatedItemsToHero()const;};
void CGameScriptInterfaceMethods::ReturnAllConfiscatedItemsToHero()const{
 CThingNative* hero=static_cast<PlayerManagerMethods*>(PlayerManager)->GetMainPlayer()->GetHeroThing();if(hero&&!(hero->DeletedFlags&1)){
if(hero->UsingComponents[0]&0x20000){GetComponent<17,InventoryMethods>(hero)->ReturnAll();}
if(hero->UsingComponents[0]&0x80000){GetComponent<19,InventoryMethods>(hero)->ReturnAll();}
if(hero->UsingComponents[0]&0x40000){GetComponent<18,InventoryMethods>(hero)->ReturnAll();}
if(hero->UsingComponents[0]&0x10){GetComponent<4,HeroStatsMethods>(hero)->SetMoneyAsConfiscated(false);}
}}