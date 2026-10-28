#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
class CThingPlayerCreature;
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };   // 0x00487DC0

extern "C" CPlayer* __fastcall GetThing(void* p);
extern float g_defaultHealthMax;


float __fastcall CGameScriptInterface_GetHeroHealthMax(CGameScriptInterface* self)
{
    CPlayer* t = GetThing(self->PlayerManager);
    CThingPlayerCreature* h = t->GetPControlledCreature();
    if (h != 0) {
        if ((*(unsigned char*)((char*)h + 0x91) & 1) == 0) {
            return *(float*)((char*)h + 0xb0);
        }
    }
    return g_defaultHealthMax;
}