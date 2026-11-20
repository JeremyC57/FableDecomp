// CGameScriptInterface::SetWeaponAsHerosActiveWeapon(const CCharString&) const -- if the named weapon definition exists and the live hero
//   carries at least one, make it the hero's active weapon
// Leaf: ?SetWeaponAsHerosActiveWeapon@CGameScriptInterface@@UBEXABVCCharString@@@Z
// NF132 (q6233): defined on the shared-header class (engine/CGameScriptInterface.h slot 122 = 0x00898B30; Ego_r.pdb
//   ?SetWeaponAsHerosActiveWeapon@CGameScriptInterface@@UBEXABVCCharString@@@Z) with the fable_string.h CCharString, instead of the
//   derived CGameScriptInterface_Methods stand-in over the CharString898b30 stand-in; DefinitionManager (+0x10) is the shared
//   engine/CGameDefinitionManager.h class, so 0x009AD410 binds ?GetDefGlobalIndexFromName@CDefinitionManager@@QBEJABVCCharString@@@Z
//   (Ego_r.pdb + FableWin.pdb; was a local CGameDefinitionManager model taking CharString898b30*).
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#include "engine/CGameDefinitionManager.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

// agentNF17: the Entry/Map stand-ins are the explicit specialisation CVectorMap<ETCInterfaceType, CTCBase*> (the landed 0x0074EE90
//   idiom) and its std::pair value_type; 0x0040F020 is its LowerBound (NF14 / established spelling).
class CTCBase;
enum ETCInterfaceType { };
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class C> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> value_type;
    value_type* _M_start;                                   // +0x00 (the STLport vector)
    value_type* _M_finish;                                  // +0x04
    value_type* LowerBound(const ETCInterfaceType& key);    // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
namespace NUISystem { class CComponent; }
class CTCInventoryWeapons { public:
    void SetThingAsActiveWeapon(long a0, NUISystem::CComponent*a1);   // 0x005CB2B0 (Ego_r.pdb ?SetThingAsActiveWeapon@CTCInventoryWeapons@@QAEXJPAVCComponent@NUISystem@@@Z)
};
 namespace NInventory { class CTCInventoryBase { public: long GetNumberOfItemsOfTypeInInventory(long index) const; }; } /* 0x005BDF08 Ego_r.pdb, const */ class CThingPlayerCreature; class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; }; class CPlayerManager { public: CPlayer& GetMainPlayer();   /* 0x00449970 */ }; struct Hero898b30;
struct Hero898b30 { unsigned char unused00[0x20]; unsigned long flags20; unsigned char unused24[0x20]; CInterfaceMap interfaces44; unsigned char unused4c[0x45]; unsigned char flags91; }; class CThingPlayerCreature : public Hero898b30 { }; void CGameScriptInterface::SetWeaponAsHerosActiveWeapon(const CCharString& weaponName) const{long index=DefinitionManager->GetDefGlobalIndexFromName(weaponName);if(index<=0)return;CPlayer* middle=(&PlayerManager->GetMainPlayer());Hero898b30* hero=middle->GetPControlledCreature();if(hero==0||(hero->flags91&1)!=0||(hero->flags20&0x80000)==0)return;CInterfaceMap* map=&(hero->interfaces44);ETCInterfaceType key=(ETCInterfaceType)0x13;CInterfaceMap::value_type* entry=map->LowerBound(key);if(entry==map->_M_finish||0x13<entry->first)entry=map->_M_finish;NInventory::CTCInventoryBase* inventory=(NInventory::CTCInventoryBase*)entry->second;if(inventory->GetNumberOfItemsOfTypeInInventory(index)>0)((CTCInventoryWeapons*)inventory)->SetThingAsActiveWeapon(index, 0);}