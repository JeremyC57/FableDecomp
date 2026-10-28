// CGameScriptInterface::TakeObjectFromHero(const CCharString&) -- remove one inventory item by definition name
// Retail00898DB0,173 bytes; native Thing bitset20,map44; category is __fastcall GetInventoryCategory005D99D0.
#include <stdio.h>
#include <stdlib.h>
#include "engine/CGameScriptInterface.h"
#include "engine/CPlayerManager.h"
#define CPlayer CPlayerPdb
#include "engine/CPlayer.h"
#undef CPlayer
#include "fable_string.h"
struct ComponentEntry{long Type;void* Component;};struct ComponentMap{ComponentEntry *Start,*Finish;ComponentEntry* LowerBound(const long&);}; //0040F020
struct CThingNative{void* VTable;unsigned char Base[0x1c];unsigned int UsingComponents[9];ComponentMap Components;};
class CThingPlayerCreature : public CThingNative { };
class CPlayer : public CPlayerPdb { public: CThingPlayerCreature* GetPControlledCreature() const; }; //00487DC0
struct CPlayerManagerMethods:CPlayerManager{CPlayer* GetMainPlayer();}; //00449970
struct DefinitionMethods{long GetDefGlobalIndexFromName(const CCharString&);}; //009AD410
long __fastcall GetInventoryCategory_005d99d0(long); //005D99D0
struct InventoryMethods{void* VTable;long GetNumberOfItemsOfTypeInInventory(long);}; //005BDF08
union InventorySlot{void* Native;void(InventoryMethods::*RemoveItem)(long,long);}; // actual inventory slot79
struct CGameScriptInterfaceMethods:CGameScriptInterface{void TakeObjectFromHero(const CCharString&)const;};
void CGameScriptInterfaceMethods::TakeObjectFromHero(const CCharString& name)const{
 long index=reinterpret_cast<DefinitionMethods*>(DefinitionManager)->GetDefGlobalIndexFromName(name);
 if(index>0){long category=GetInventoryCategory_005d99d0(index);if(category){CThingPlayerCreature* hero=static_cast<CPlayerManagerMethods*>(PlayerManager)->GetMainPlayer()->GetPControlledCreature();InventoryMethods* inventory=0;
 if(static_cast<unsigned long>(category)>=274){puts("bitset");abort();}
 if(hero->UsingComponents[static_cast<unsigned long>(category)>>5]&(1U<<(category&31))){ComponentMap* map=&hero->Components;long key=category;ComponentEntry* it=map->LowerBound(key);ComponentEntry* end=map->Finish;inventory=static_cast<InventoryMethods*>((it==end||category<it->Type?end:it)->Component);}
 if(inventory->GetNumberOfItemsOfTypeInInventory(index)>0)(inventory->*reinterpret_cast<InventorySlot*>(inventory->VTable)[79].RemoveItem)(index,1);
 }}
}