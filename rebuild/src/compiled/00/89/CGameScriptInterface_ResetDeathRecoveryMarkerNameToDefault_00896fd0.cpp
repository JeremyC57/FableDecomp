// CGameScriptInterface::ResetDeathRecoveryMarkerNameToDefault
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct MapNode;
class CThingPlayerCreature;
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };   // 0x00487DC0
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
struct SubB;
struct MapKind {
    int _pad0;             // +0x00
    struct MapNode* head;  // +0x04
    struct MapNode* lb(int* key);   // __fastcall lower_bound-like
};
struct Node2 { void* p; };

extern "C" void __fastcall FinalizeNode(void* n);      // this in ecx

void __fastcall CGameScriptInterface_ResetDeathRecoveryMarkerNameToDefault(CGameScriptInterface* self)
{
    int key;
    CPlayer* a = (&self->PlayerManager->GetMainPlayer());
    CThingPlayerCreature* obj = a->GetPControlledCreature();
    if (!obj) return;
    if (*(unsigned char*)((char*)obj + 0x91) & 1) return;
    if (!(*(unsigned char*)((char*)obj + 0x20) & 0x10)) return;

    MapKind* m = (MapKind*)((char*)obj + 0x44);
    key = 4;
    MapNode* found = m->lb(&key);
    MapNode* endn = m->head;
    if (found == endn || *(int*)found > 4) {
        found = endn;
    }
    FinalizeNode(*(void**)((char*)found + 4));
}