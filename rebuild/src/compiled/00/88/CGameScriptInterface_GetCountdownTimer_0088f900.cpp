#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
class CWorld { public: char pad[0xa0]; int frames; };

float __fastcall CGameScriptInterface_GetCountdownTimer(const CGameScriptInterface* self)
{
    return (float)self->World->frames;
}