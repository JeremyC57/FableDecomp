// CGameScriptInterface::ResetWhetherToFailQuestOnDeathToDefault
// __fastcall, void, 1 param (this)

#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
typedef std::pair<ETCInterfaceType, CTCBase*> MapNode;

template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    void* root;      // +0x00
    MapNode* header; // +0x04  (end sentinel)
    MapNode* LowerBound(const ETCInterfaceType& keyPtr); // thiscall: this in ecx, keyPtr on stack
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > Map;

class CThingPlayerCreature;
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };   // 0x00487DC0
extern CPlayer* __fastcall GetSubObject(void* self);
extern void __fastcall Consume(void* value);


void __fastcall CGameScriptInterface_ResetWhetherToFailQuestOnDeathToDefault(CGameScriptInterface* self)
{
    CPlayer* a = GetSubObject(self->PlayerManager);
    unsigned char* eax = (unsigned char*)a->GetPControlledCreature();
    if (!eax) return;
    if (eax[0x91] & 0x1) return;
    if (!(eax[0x20] & 0x10)) return;

    Map* map = (Map*)((char*)eax + 0x44);
    ETCInterfaceType key = (ETCInterfaceType)(4);
    MapNode* node = map->LowerBound(key);
    MapNode* endNode = map->header;
    MapNode* sel;
    if (node == endNode) sel = endNode;
    else if (node->first <= 4) sel = node;
    else sel = endNode;
    Consume(((void*)sel->second));
}