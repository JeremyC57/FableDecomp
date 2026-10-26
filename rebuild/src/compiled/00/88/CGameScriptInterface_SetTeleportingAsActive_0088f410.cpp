#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
class CWorld { public: unsigned char pad[0xd5]; unsigned char teleporting; };
void __fastcall CGameScriptInterface_SetTeleportingAsActive(CGameScriptInterface* self, int edx, char active) {
    self->World->teleporting = active;
}