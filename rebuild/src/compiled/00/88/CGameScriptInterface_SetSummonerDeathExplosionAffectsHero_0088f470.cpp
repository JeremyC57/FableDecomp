#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
class CWorld { public: unsigned char _pad[0xe1]; unsigned char flagE1; };

void __fastcall CGameScriptInterface_SetSummonerDeathExplosionAffectsHero(CGameScriptInterface* self, int /*edx*/, char doesAffect)
{
    self->World->flagE1 = doesAffect;
}