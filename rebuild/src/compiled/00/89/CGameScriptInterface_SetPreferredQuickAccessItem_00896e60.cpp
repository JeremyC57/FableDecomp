#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct CharString896e60 { void* data; };

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
struct Inventory896e60 { void AddContextSensitivePreferredItem(const CharString896e60& name,long context,int slot); void RemoveContextSensitivePreferredItem(const CharString896e60& name); }; class CThingPlayerCreature; class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; }; class CPlayerManager { public: CPlayer& GetMainPlayer();   /* 0x00449970 */ }; struct Target896e60;
struct Target896e60 { unsigned char unused00[0x20]; unsigned long flags20; unsigned char unused24[0x20]; CInterfaceMap interfaces44; unsigned char unused4c[0x45]; unsigned char flags91; }; class CThingPlayerCreature : public Target896e60 { }; struct CGameScriptInterface_Methods : CGameScriptInterface {
    void SetPreferredQuickAccessItem(const CharString896e60& itemName,long context,int slot) const;
};
void CGameScriptInterface_Methods::SetPreferredQuickAccessItem(const CharString896e60& itemName,long context,int slot) const{CPlayer* middle=(&PlayerManager->GetMainPlayer());Target896e60* target=middle->GetPControlledCreature();if(target==0||(target->flags91&1)!=0||(target->flags20&0x20000)==0)return;CInterfaceMap* map=&(target->interfaces44);ETCInterfaceType key=(ETCInterfaceType)0x11;CInterfaceMap::value_type* entry=map->LowerBound(key);if(entry==map->_M_finish||0x11<entry->first)entry=map->_M_finish;Inventory896e60* inventory=(Inventory896e60*)entry->second;if(inventory==0)return;if(slot>-1){inventory->AddContextSensitivePreferredItem(itemName,context,slot);return;}inventory->RemoveContextSensitivePreferredItem(itemName);}