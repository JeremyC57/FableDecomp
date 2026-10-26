#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
class CWorld { public: char pad[0xd5]; bool active; };

bool __fastcall CGameScriptInterface_IsTeleportingActive(const CGameScriptInterface* self)
{
    return self->World->active;
}