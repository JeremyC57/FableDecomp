#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
struct Inner2 { char _p[0x14]; long f14; };
class CWorld { public: char _p[0x1c]; Inner2* PGameTimeManager; };

long __fastcall CGameScriptInterface_GetDayOfWeek(const CGameScriptInterface* self)
{
    CWorld* m = self->World;
    Inner2* x = m->PGameTimeManager;
    return x->f14 % 7;
}