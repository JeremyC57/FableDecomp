// CThing::~CThing() -- thing destructor: drop the thing count, purge this thing's entries from CThing::TCsToBeRemoved, delete every component
//   (clearing its presence bit), empty the component map and unregister from the current thing manager; then the members' and CClass /
//   CBaseObject's destruction
// Leaf: ??1CThing@@MAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C8310, 412 bytes (the bitset range-error block at 0x004C849A ends it); vtable store 0x01239ABC.
// Name: Ego_r.pdb `??1CThing@@MAE@XZ` (protected virtual) + FableWin.pdb; locals from Ego_d.pdb (egod_locals.py): `remove_tc_it` (its own
//   scope) and `n` (long, compared against the unsigned size(): `jbe` / `jb`).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 and ~CBaseIntelligentPointer 0x00A01C10 (list erase), free 0x00BFEA14, the
//   component's slots 21 GetInterfaceType and 0 (scalar deleting destructor), CClass::Unregister 0x00A37160 on CThing::PCurrentThingManager
//   0x013B8A1C, ~CCharString 0x0099EAE0 (x3), CBaseObject::~CBaseObject 0x0099A430 (tail jump), puts / abort (STLport's
//   __stl_throw_out_of_range("bitset") from the inline bitset::reset).
// Data: CThing::TCsToBeRemoved 0x013B8A24 (q11218 spelling); the thing counter 0x013B8A20 keeps the landed constructor's descriptive name
//   g_thingCount (Ego_d.pdb PUB32 ?NoThings@CThing@@2JA is one source; NOTES).
// Port (NF92, q11218 binder): was a local model of STLport (list / vector / bitset), CTCBase, CVectorMap, CCharString, CDefPointer, CClass
//   and a 30-member CThing binding 0x013B8A24 under its own type; now on the shared headers with NF92 opt-ins FABLE_RETAIL_CTHING_DTOR
//   (engine/CThing.h, the destructor's declaration) and FABLE_RETAIL_STLP_BITSET_RESET (stlport/fable_containers.h, reset inline).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHING_TCSTOBEREMOVED
#define FABLE_RETAIL_CTHING_DTOR
#define FABLE_RETAIL_CTHING_MANAGERACCESS
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHINGMANAGER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_BITSET_RESET
#include "fable_string.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingManager.h"

extern long g_thingCount;                                   // 0x013B8A20

CThing::~CThing()
{
    --g_thingCount;
    AddingTCsPermitted = false;
    {
        std::list<std::pair<CIntelligentPointer<CThing>, CTCBase*> >::iterator remove_tc_it = TCsToBeRemoved.begin();
        while (remove_tc_it != TCsToBeRemoved.end()) {
            if (remove_tc_it->first.GetPItem() == this)
                remove_tc_it = TCsToBeRemoved.erase(remove_tc_it);
            else
                ++remove_tc_it;
        }
    }
    for (long n = 0; n < Components.size(); ++n) {
        UsingComponents.reset(Components[n].second->GetInterfaceType());
        delete Components[n].second;
        Components[n].second = 0;
    }
    AddingTCsPermitted = true;
    Components.clear();
    Unregister(*GetPCurrentThingManager());
}
