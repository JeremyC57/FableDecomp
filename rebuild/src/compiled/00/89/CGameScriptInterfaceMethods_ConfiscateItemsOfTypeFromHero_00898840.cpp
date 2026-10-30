// CGameScriptInterface::ConfiscateItemsOfTypeFromHero -- confiscate present hero inventories
// Retail00898840,245 bytes.
#include "engine/CGameScriptInterface.h"
#include "engine/CPlayerManager.h"
#define CPlayer CPlayerPdb
#include "engine/CPlayer.h"
#undef CPlayer
#include "engine/CTCHeroStats.h"
#include "fable_string.h"
#include "engine/_quarantine/CTCInventoryBase.h"
struct ComponentEntry{long Type;void* Component;};struct ComponentMap{ComponentEntry *Start,*Finish,*Capacity;ComponentEntry* LowerBound(const long&);}; //0040F020
struct CThingNative{void* VTable;unsigned char Base[0x1c];unsigned int UsingComponents[9];ComponentMap Components;unsigned char Rest[0x41];unsigned char DeletedFlags;};
class CThingPlayerCreature : public CThingNative { };
class CPlayer : public CPlayerPdb { public: CThingPlayerCreature* GetPControlledCreature() const; }; //00487DC0
struct InventoryMethods:CTCInventoryBase{void ConfiscateAllItems();void ConfiscateAllItemsOfType(long);}; //005BFA8E,005BFAC6; actual quarantined donor header; this leaf uses no discrepant inventory fields
struct HeroStatsMethods:CTCHeroStats{void SetMoneyAsConfiscated(bool);}; //0057C999
class CDefinitionManager
{
public:
    long GetDefGlobalIndexFromName(const CCharString&) const;   // 0x009AD410
};

class CGameDefinitionManager: public CDefinitionManager { public:static CGameDefinitionManager& __fastcall Get();}; //0044C6B0,009AD410
template<int Key,class T> __forceinline T* GetComponent(CThingNative* thing){T* result;if(thing->UsingComponents[0]&(1u<<Key)){ComponentMap* map=&thing->Components;ComponentEntry* it=map->LowerBound(Key);ComponentEntry* end=map->Finish;result=static_cast<T*>((it==end||it->Type>Key?end:it)->Component);}return result;}
struct CGameScriptInterfaceMethods:CGameScriptInterface{void ConfiscateItemsOfTypeFromHero(const CCharString& type)const;};
void CGameScriptInterfaceMethods::ConfiscateItemsOfTypeFromHero(const CCharString& type)const{
 CThingPlayerCreature* hero=PlayerManager->GetMainPlayer().GetPControlledCreature();if(hero&&!(hero->DeletedFlags&1)){
long index=CGameDefinitionManager::Get().GetDefGlobalIndexFromName(type);if(index>0){
if(hero->UsingComponents[0]&0x20000){GetComponent<17,InventoryMethods>(hero)->ConfiscateAllItemsOfType(index);}
if(hero->UsingComponents[0]&0x80000){GetComponent<19,InventoryMethods>(hero)->ConfiscateAllItemsOfType(index);}
if(hero->UsingComponents[0]&0x40000){GetComponent<18,InventoryMethods>(hero)->ConfiscateAllItemsOfType(index);}
}
}}