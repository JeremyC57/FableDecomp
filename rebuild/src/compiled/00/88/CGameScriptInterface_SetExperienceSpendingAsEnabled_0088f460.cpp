#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
class CWorld { public: unsigned char _pad[0xd6]; unsigned char flag; };
void __fastcall CGameScriptInterface_SetExperienceSpendingAsEnabled(CGameScriptInterface* self, void* /*edx*/, unsigned char enabled) {
    self->World->flag = enabled;
}