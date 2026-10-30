#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct HealthObj { char pad[0x91]; unsigned char flags; char pad2[0xb4-0x92]; float health; };
class CThingPlayerCreature : public HealthObj { };
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };   // 0x00487DC0

class CPlayerManager { public: CPlayer& GetMainPlayer(); }; /* 0x00449970 CPlayerManager::GetMainPlayer (Ego_r 0x004450F0) */
extern float g_defaultHealth;

float __fastcall CGameScriptInterface_GetHeroHealth(CGameScriptInterface* self)
{
    HealthObj* h = (&((CPlayerManager*)(*(int*)((char*)self + 0x14)))->GetMainPlayer())->GetPControlledCreature();
    if (h != 0 && (h->flags & 1) == 0)
        return h->health;
    return g_defaultHealth;
}