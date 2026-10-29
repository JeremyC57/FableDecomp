// CGameScriptInterface::RemoveAllHeroWeapons() -- snapshot inventory entries, remove each, then reset active weapons
// Retail00898600; native vector3 pointers and pair<long,long> entries; removal takes two longs; destructor uses free BFEA14.
#include <stdlib.h>
#include "engine/CGameScriptInterface.h"
#include "engine/CPlayerManager.h"
#define CPlayer CPlayerPdb
#include "engine/CPlayer.h"
#undef CPlayer
#include "fable_string.h"
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
typedef std::pair<ETCInterfaceType, CTCBase*> ComponentEntry;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:ComponentEntry *Start,*Finish;ComponentEntry* LowerBound(const ETCInterfaceType& key);};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > ComponentMap; // 004365B0 names,0040F020 other uses: identical body
// Native CThing map+44; donor+48. Life flag+91 and inventory19 bit are verified in retail.
struct CThingNative {void* VTable;unsigned char Base[0x1c];unsigned int UsingComponents[9];ComponentMap Components;unsigned char State[0x45];unsigned char LifeFlags;};
class CThingPlayerCreature : public CThingNative { };
class CPlayer : public CPlayerPdb { public: CThingPlayerCreature* GetPControlledCreature() const; }; //00487DC0
struct CPlayerManagerMethods:CPlayerManager {CPlayer* GetMainPlayer();}; //00449970
struct ItemPair {long Index,Count;};
struct ItemVector {ItemPair *Start,*Finish,*End;ItemVector():Start(0),Finish(0),End(0){}~ItemVector(){if(Start)free(Start);}};
struct CTCInventoryWeaponsNative {void* VTable;void GetAllItems(ItemVector&);}; //005BF482
union InventorySlot {void* Native;void(CTCInventoryWeaponsNative::*RemoveItem)(long,long);void(CTCInventoryWeaponsNative::*Clear)();}; // native slots79 and74
struct CGameScriptInterfaceMethods:CGameScriptInterface {void RemoveAllHeroWeapons()const;};
void CGameScriptInterfaceMethods::RemoveAllHeroWeapons()const{
 CThingPlayerCreature* hero=static_cast<CPlayerManagerMethods*>(PlayerManager)->GetMainPlayer()->GetPControlledCreature();
 if(hero && !(hero->LifeFlags&1) && (hero->UsingComponents[0]&0x80000)) {
 ComponentMap* map=&hero->Components;ComponentEntry* it=map->LowerBound((ETCInterfaceType)19);ComponentEntry* end=map->Finish;
 CTCInventoryWeaponsNative* inventory=static_cast<CTCInventoryWeaponsNative*>(((void*)(it==end||it->first>19?end:it)->second));
 ItemVector items;inventory->GetAllItems(items);
 for(ItemPair* p=items.Start;p!=items.Finish;++p)(inventory->*reinterpret_cast<InventorySlot*>(inventory->VTable)[79].RemoveItem)(p->Index,p->Count);
 (inventory->*reinterpret_cast<InventorySlot*>(inventory->VTable)[74].Clear)();
 }
}