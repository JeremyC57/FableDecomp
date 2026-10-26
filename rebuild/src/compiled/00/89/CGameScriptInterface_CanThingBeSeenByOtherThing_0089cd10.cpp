#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified
class CThing;
struct CGameScriptInterface_Methods : CGameScriptInterface {
    bool CanThingBe_Seen_ByOtherThing(CThing* thing1, CThing* thing2);
    bool CanSeeImpl(CThing* thing1, CThing* thing2, int mode);
};

bool CGameScriptInterface_Methods::CanThingBe_Seen_ByOtherThing(CThing* thing1, CThing* thing2)
{
    return CanSeeImpl(thing1, thing2, 1);
}