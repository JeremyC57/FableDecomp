// NTCScriptedControl::CActionFollowCreature::SetAsSneaking(bool sneaking) -- switch the controlled creature's sneaking on (adding a CTCStealth component when it has none, flagging it, and entering creature mode 7 when it was not sneaking yet) or off (clearing the flag, leaving mode 7, and removing the stealth component this action added)
// Leaf: ?SetAsSneaking@CActionFollowCreature@NTCScriptedControl@@AAEX_N@Z
// Retail: Fable.exe 0x00713D50, 395 bytes, thiscall `ret 4` (Ego_r.pdb name above; caller Update 0x00713EE0).
// Callees: CCharString(const char*, long) 0x0099EBF0 ("CTCStealth" 0x0123AB08) / ~CCharString 0x0099EAE0, CThing::AddTC 0x004C9D60,
//   CVectorMap::LowerBound 0x0040F020 (the expanded GetTC<CTCStealth> / GetTC<CTCCreatureModeManager>), CTCCreatureModeManager::AddMode
//   0x0073AE10 / RemoveMode 0x0073ABA0, CThing::RemoveTC 0x004C9840.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CTCSCRIPTEDCONTROL_ACTIONBASE
#define FABLE_RETAIL_CTCSTEALTH_SETSNEAKING
#define FABLE_RETAIL_CTCSTEALTH_SNEAKTOGGLE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCStealth.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/NTCScriptedControl_CActionFollowCreature.h"

namespace NTCScriptedControl {

void CActionFollowCreature::SetAsSneaking(bool sneaking)
{
    if (Sneaking == sneaking)
        return;
    if (sneaking) {
        if (!GetParentThing().HasTC(TCI_STEALTH)) {
            GetParentThing().AddTC("CTCStealth", false, 0);
            AddedStealthTC = true;
        }
        CTCStealth* stealth;
        GetParentThing().GetTC(&stealth);
        if (!stealth->IsSneaking()) {
            stealth->SetSneaking(true);
            CTCCreatureModeManager* modes;
            if (GetParentThing().GetTC(&modes))
                modes->AddMode(7);
        }
        Sneaking = true;
    } else {
        CTCStealth* stealth;
        GetParentThing().GetTC(&stealth);
        stealth->SetSneaking(false);
        CTCCreatureModeManager* modes;
        if (GetParentThing().GetTC(&modes))
            modes->RemoveMode(7);
        if (AddedStealthTC) {
            GetParentThing().RemoveTC(TCI_STEALTH);
            AddedStealthTC = false;
        }
        Sneaking = false;
    }
}

}