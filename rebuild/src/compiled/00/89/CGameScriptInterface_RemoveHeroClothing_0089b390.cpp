// CGameScriptInterface::RemoveHeroClothing @ 0089b390
#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct Node { int key; void* val; };

struct Vec {
    Node* begin;   // +0x00
    Node* end;     // +0x04
    Node* lower_bound(int* key); // __fastcall, key pushed as stack arg
};

class CPlayer;
class CPlayerManager { public: CPlayer& GetMainPlayer(); }; /* 0x00449970 CPlayerManager::GetMainPlayer (Ego_r 0x004450F0) */
extern void  __fastcall DoRemove(void* self);    // 0xffd18d56

struct HeroData {
    char pad[0x20];
    unsigned int flags20; // +0x20
    char pad2[0x91 - 0x24];
    unsigned char flag91;  // +0x91
    // Vec at +0x44
};
class CThingPlayerCreature : public HeroData { };
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };   // 0x00487DC0


void __fastcall CGameScriptInterface_RemoveHeroClothing(CGameScriptInterface* self)
{
    int key;
    CPlayer* h = (&((CPlayerManager*)self->PlayerManager)->GetMainPlayer());
    HeroData* d = h->GetPControlledCreature();
    if (!d) return;
    if (d->flag91 & 1) return;
    if (!(d->flags20 & 0x40000)) return;
    Vec* vec = (Vec*)((char*)d + 0x44);
    key = 0x12;
    Node* it = vec->lower_bound(&key);
    Node* end = vec->end;
    if (it != end && it->key <= 0x12) {
        // keep it
    } else {
        it = end;
    }
    DoRemove(it->val);
}