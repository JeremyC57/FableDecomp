// CGameScriptInterface::TakeObjectFromHero(const CCharString&) -- remove one inventory item by definition name
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail00898DB0,173 bytes; native Thing bitset20,map44; category is __fastcall GetInventoryCategory005D99D0.
#include <stdio.h>
#include <stdlib.h>
#include "engine/CGameScriptInterface.h"
#include "engine/CPlayerManager.h"
#define CPlayer CPlayerPdb
#include "engine/CPlayer.h"
#undef CPlayer
#include "fable_string.h"
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef std::pair<ETCInterfaceType, CTCBase*> ComponentEntry;template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:ComponentEntry *Start,*Finish;ComponentEntry* LowerBound(const ETCInterfaceType& key);};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > ComponentMap; //0040F020
struct CThingNative{void* VTable;unsigned char Base[0x1c];unsigned int UsingComponents[9];ComponentMap Components;};
class CThingPlayerCreature : public CThingNative { };
class CPlayer : public CPlayerPdb { public: CThingPlayerCreature* GetPControlledCreature() const; }; //00487DC0
struct DefinitionMethods{long GetDefGlobalIndexFromName(const CCharString&);}; //009AD410
long __fastcall GetInventoryCategory_005d99d0(long); //005D99D0
namespace NInventory{class CTCInventoryBase{public:void* VTable;long GetNumberOfItemsOfTypeInInventory(long)const;};} //005BDF08 Ego_r.pdb ?GetNumberOfItemsOfTypeInInventory@CTCInventoryBase@NInventory@@QBEJJ@Z
using NInventory::CTCInventoryBase;
union InventorySlot{void* Native;void(CTCInventoryBase::*RemoveItem)(long,long);}; // actual inventory slot79
struct CGameScriptInterfaceMethods:CGameScriptInterface{void TakeObjectFromHero(const CCharString&)const;};
void CGameScriptInterfaceMethods::TakeObjectFromHero(const CCharString& name)const{
 long index=reinterpret_cast<DefinitionMethods*>(DefinitionManager)->GetDefGlobalIndexFromName(name);
 if(index>0){long category=GetInventoryCategory_005d99d0(index);if(category){CThingPlayerCreature* hero=PlayerManager->GetMainPlayer().GetPControlledCreature();CTCInventoryBase* inventory=0;
 if(static_cast<unsigned long>(category)>=274){puts("bitset");abort();}
 if(hero->UsingComponents[static_cast<unsigned long>(category)>>5]&(1U<<(category&31))){ComponentMap* map=&hero->Components;ETCInterfaceType key = (ETCInterfaceType)(category);ComponentEntry* it=map->LowerBound(key);ComponentEntry* end=map->Finish;inventory=static_cast<CTCInventoryBase*>(((void*)(it==end||category<it->first?end:it)->second));}
 if(inventory->GetNumberOfItemsOfTypeInInventory(index)>0)(inventory->*reinterpret_cast<InventorySlot*>(inventory->VTable)[79].RemoveItem)(index,1);
 }}
}