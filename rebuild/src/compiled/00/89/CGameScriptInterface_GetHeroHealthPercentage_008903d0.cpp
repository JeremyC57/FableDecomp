#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
class CThingPlayerCreature;
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };   // 0x00487DC0

extern CPlayer* __fastcall Sub_GetManager(void* self);
extern float __fastcall Sub_GetHealthPct(CThingPlayerCreature* hero);
extern float g_defaultHealthPct;

float __fastcall CGameScriptInterface_GetHeroHealthPercentage(CGameScriptInterface* self)
{
    void* mgr = self->PlayerManager;
    CThingPlayerCreature* hero = Sub_GetManager(mgr)->GetPControlledCreature();
    if (hero != 0 && (*(unsigned char*)((char*)hero + 0x91) & 0x1) == 0) {
        float v = Sub_GetHealthPct(hero);
        return v;
    }
    return g_defaultHealthPct;
}