// CGameScriptInterface::ConfiscateAllHeroItems -- confiscate present hero inventories
// Retail008986d0,262 bytes.
#include "fable_compiler.h"
#include "engine/CGameScriptInterface.h"
#include "engine/CPlayerManager.h"
#define CPlayer CPlayerPdb
#include "engine/CPlayer.h"
#undef CPlayer
#include "engine/CTCHeroStats.h"
#include "fable_string.h"
#include "engine/_quarantine/CTCInventoryBase.h"
// agentNF17: the ComponentEntry / ComponentMap stand-ins are the explicit specialisation CVectorMap<ETCInterfaceType, CTCBase*> (the
//   landed 0x0074EE90 idiom) and its std::pair value_type; 0x0040F020 is its LowerBound (NF14 / established spelling).
class CTCBase;
enum ETCInterfaceType { };
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class C> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> value_type;
    value_type *_M_start,*_M_finish,*_M_end_of_storage;                     // the STLport vector
    value_type* LowerBound(const ETCInterfaceType& key);    // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > ComponentMap;
typedef ComponentMap::value_type ComponentEntry;
struct CThingNative{void* VTable;unsigned char Base[0x1c];unsigned int UsingComponents[9];ComponentMap Components;unsigned char Rest[0x41];unsigned char DeletedFlags;};
class CThingPlayerCreature : public CThingNative { };
class CPlayer : public CPlayerPdb { public: CThingPlayerCreature* GetPControlledCreature() const; }; //00487DC0
struct InventoryMethods:CTCInventoryBase{void ConfiscateAllItems();void ConfiscateAllItemsOfType(long);}; //005BFA8E,005BFAC6; actual quarantined donor header; this leaf uses no discrepant inventory fields
struct HeroStatsMethods:CTCHeroStats{void SetMoneyAsConfiscated(bool);}; //0057C999
class CGameDefinitionManager{ public:static CGameDefinitionManager& __fastcall Get();long GetDefGlobalIndexFromName(const CCharString&);}; //0044C6B0,009AD410
template<int Key,class T> FABLE_FORCEINLINE T* GetComponent(CThingNative* thing){T* result;if(thing->UsingComponents[0]&(1u<<Key)){ComponentMap* map=&thing->Components;ComponentEntry* it=map->LowerBound((ETCInterfaceType)Key);ComponentEntry* end=map->_M_finish;result=(T*)((it==end||it->first>Key?end:it)->second);}return result;}
struct CGameScriptInterfaceMethods:CGameScriptInterface{void ConfiscateAllHeroItems()const;};
void CGameScriptInterfaceMethods::ConfiscateAllHeroItems()const{
 CThingPlayerCreature* hero=PlayerManager->GetMainPlayer().GetPControlledCreature();if(hero&&!(hero->DeletedFlags&1)){
if(hero->UsingComponents[0]&0x20000){GetComponent<17,InventoryMethods>(hero)->ConfiscateAllItems();}
if(hero->UsingComponents[0]&0x80000){GetComponent<19,InventoryMethods>(hero)->ConfiscateAllItems();}
if(hero->UsingComponents[0]&0x40000){GetComponent<18,InventoryMethods>(hero)->ConfiscateAllItems();}
if(hero->UsingComponents[0]&0x10){GetComponent<4,HeroStatsMethods>(hero)->SetMoneyAsConfiscated(true);}
}}