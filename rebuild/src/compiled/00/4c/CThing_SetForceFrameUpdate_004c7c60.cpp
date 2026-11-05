// CThing::SetForceFrameUpdate(bool force) -- set the force-frame-update flag; when it ends up set on a live thing, put the thing on the thing manager's update list
// Leaf: ?SetForceFrameUpdate@CThing@@QAEX_N@Z
// Naming wave 18: the global stand-in g_pThingManager (0x013B8A1C) is the private static CThing::PCurrentThingManager, read through the inline
//   static accessor GetPCurrentThingManager() as retail code outside CThing does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C7C60, 58 bytes. Callees: CThingManager::AddToUpdateList(CThing*)
// @ 0x0051EC50 on the global thing manager @ 0x013B8A1C (tail jump, `this` stored over the argument slot).
// Shape: ForceFrameUpdate is bit 3 of +0x92 (merge store, then the stored byte re-tested: `mov al,dl ; test al,8`);
// Dead is bit 0 of +0x91.
class CThing;

class CThingManager { public:
    void AddToUpdateList(CThing* thing);             // 0x0051EC50
};

#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MANAGERACCESS
#define FABLE_RETAIL_CTHING_STATEACCESS
#include "engine/CThing.h"

void CThing::SetForceFrameUpdate(bool force)
{
    ForceFrameUpdate = force;
    if (ForceFrameUpdate && !Dead)
        CThing::GetPCurrentThingManager()->AddToUpdateList(this);
}