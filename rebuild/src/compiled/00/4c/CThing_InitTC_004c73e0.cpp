// CThing::InitTC(CTCBase* pbase) -- bring a newly added thing component up to the thing's own initialisation stage (stage 1, 2 or 3 of the
//   TC's staged initialisers OnCreate / InitialActivate / OnInitialActivate) unless it is already marked for removal
// Leaf: ?InitTC@CThing@@AAEXPAVCTCBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C73E0, 86 bytes (+ int3 padding). No direct callees: CTCBase virtual slots 4 OnCreate, 5
//   InitialActivate, 6 OnInitialActivate (+0x10 / +0x14 / +0x18).
// Shape: CTCBase::ToBeRemoved is the bool at +8; CThing::InitialisationStage is the 3-bit field at retail +0x94, read once (`and al,7`) and
//   compared with 1, 2, 3 in an if/else chain (a switch lowers to `dec`); every arm keeps its own `pop esi ; ret 4`.
// Identity (NF92, q11217): Ego_r.pdb 0x004C94D0 + FableWin.pdb 0x01CF5BD0 ?InitTC@CThing@@AAEXPAVCTCBase@@@Z (private); the ported
//   CThing::AddTC 0x004C9D60 binds it so (NF90 opt-in FABLE_RETAIL_CTHING_INITTC). Ego_d.pdb names the parameter `pbase`. Was a local
//   CThing / CTCBase model (public: leaf ?InitTC@CThing@@QAEXPAVCTCBase@@@Z); now on engine/CThing.h + engine/CTCBase.h.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_SERIALISE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_INITTC
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"

void CThing::InitTC(CTCBase* pbase)
{
    if (!pbase->IsToBeRemoved()) {
        unsigned char stage = InitialisationStage;
        if (stage == 1) {
            pbase->OnCreate();
        } else if (stage == 2) {
            pbase->OnCreate();
            pbase->InitialActivate();
        } else if (stage == 3) {
            pbase->OnCreate();
            pbase->InitialActivate();
            pbase->OnInitialActivate();
        }
    }
}
