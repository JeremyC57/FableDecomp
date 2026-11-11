#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct CharString898b30 { void* data; };

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
class CGameDefinitionManager { public: long GetDefGlobalIndexFromName(const CharString898b30* name); };namespace NUISystem { class CComponent; }
class CTCInventoryWeapons { public:
    void SetThingAsActiveWeapon(long a0, NUISystem::CComponent*a1);   // 0x005CB2B0 (Ego_r.pdb ?SetThingAsActiveWeapon@CTCInventoryWeapons@@QAEXJPAVCComponent@NUISystem@@@Z)
};
 namespace NInventory { class CTCInventoryBase { public: long GetNumberOfItemsOfTypeInInventory(long index) const; }; } /* 0x005BDF08 Ego_r.pdb, const */ class CThingPlayerCreature; class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; }; class CPlayerManager { public: CPlayer& GetMainPlayer();   /* 0x00449970 */ }; struct Hero898b30;
struct Hero898b30 { unsigned char unused00[0x20]; unsigned long flags20; unsigned char unused24[0x20]; CInterfaceMap interfaces44; unsigned char unused4c[0x45]; unsigned char flags91; }; class CThingPlayerCreature : public Hero898b30 { }; struct CGameScriptInterface_Methods : CGameScriptInterface {
    void SetWeaponAsHerosActiveWeapon(const CharString898b30& weaponName) const;
};
void CGameScriptInterface_Methods::SetWeaponAsHerosActiveWeapon(const CharString898b30& weaponName) const{long index=DefinitionManager->GetDefGlobalIndexFromName(&(weaponName));if(index<=0)return;CPlayer* middle=(&PlayerManager->GetMainPlayer());Hero898b30* hero=middle->GetPControlledCreature();if(hero==0||(hero->flags91&1)!=0||(hero->flags20&0x80000)==0)return;CInterfaceMap* map=&(hero->interfaces44);ETCInterfaceType key=(ETCInterfaceType)0x13;CInterfaceMap::value_type* entry=map->LowerBound(key);if(entry==map->_M_finish||0x13<entry->first)entry=map->_M_finish;NInventory::CTCInventoryBase* inventory=(NInventory::CTCInventoryBase*)entry->second;if(inventory->GetNumberOfItemsOfTypeInInventory(index)>0)((CTCInventoryWeapons*)inventory)->SetThingAsActiveWeapon(index, 0);}