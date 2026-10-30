// CGameScriptInterface::IsPlayerCreatureBlocking @ 00899ba0
struct Node { int key; void* val; };

class CThingPlayerCreature;
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };   // 0x00487DC0
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
 /* 0x00449970 CPlayerManager::GetMainPlayer (Ego_r 0x004450F0) */
struct KeyArg { int* p; };
extern Node* __fastcall LowerBound(void* tree, KeyArg k);
extern char  __fastcall CheckBlocking(void* obj);

bool __fastcall CGameScriptInterface_IsPlayerCreatureBlocking(void* self)
{
    CPlayer* mgr = (&((CPlayerManager*)(*(void**)((char*)self + 0x14)))->GetMainPlayer());
    CThingPlayerCreature* e = mgr->GetPControlledCreature();
    if (e && !(*(unsigned char*)((char*)e + 0x91) & 1)
          && (*(unsigned int*)((char*)e + 0x24) & 0x10000))
    {
        char* tree = (char*)e + 0x44;
        int key = 0x30;
        KeyArg ka; ka.p = &key;
        Node* found = LowerBound(tree, ka);
        Node* end = *(Node**)(tree + 4);
        if (found == end || found->key > 0x30)
            found = end;
        if (CheckBlocking(*(void**)((char*)found + 4)))
            return true;
    }
    return false;
}