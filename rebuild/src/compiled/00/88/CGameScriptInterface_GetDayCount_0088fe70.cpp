#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct Inner2 { unsigned char _p[0x14]; long day; };
class CWorld { public: unsigned char _p[0x1c]; Inner2* PGameTimeManager; };

long __fastcall CGameScriptInterface_GetDayCount(const CGameScriptInterface* self) {
    return self->World->PGameTimeManager->day;
}