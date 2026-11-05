// CGameScriptInterface::ResetWhetherToFailQuestOnDeathToDefault
// Plan phase 5 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the local map's pointer members root/header are the header CArray base's begin() / end() (STLport vector start +0 / finish +4).
// __fastcall, void, 1 param (this)

#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef std::pair<ETCInterfaceType, CTCBase*> MapNode;

typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > Map;

class CThingPlayerCreature;
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };   // 0x00487DC0
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
 /* 0x00449970 CPlayerManager::GetMainPlayer (Ego_r 0x004450F0) */
extern void __fastcall Consume(void* value);


void __fastcall CGameScriptInterface_ResetWhetherToFailQuestOnDeathToDefault(CGameScriptInterface* self)
{
    CPlayer* a = (&((CPlayerManager*)self->PlayerManager)->GetMainPlayer());
    unsigned char* eax = (unsigned char*)a->GetPControlledCreature();
    if (!eax) return;
    if (eax[0x91] & 0x1) return;
    if (!(eax[0x20] & 0x10)) return;

    Map* map = (Map*)((char*)eax + 0x44);
    ETCInterfaceType key = (ETCInterfaceType)(4);
    MapNode* node = map->LowerBound(key);
    MapNode* endNode = map->end();
    MapNode* sel;
    if (node == endNode) sel = endNode;
    else if (node->first <= 4) sel = node;
    else sel = endNode;
    Consume(((void*)sel->second));
}