#pragma optimize("s",on)
// CMainGameComponent::EventIsSystemEvent(const CGameEvent* event) -- true for a system event (type 2)
// Leaf: ?EventIsSystemEvent@CMainGameComponent@@IAE_NPBVCGameEvent@@@Z
// Retail: Fable.exe @ 0x00415FF2, 19 bytes, `ret 4`; the single-case switch lowers to `dec eax ; dec eax ; je` (VC7.1).
// Name: Ego_r.pdb 0x004151A6 / FableWin.pdb 0x018A8762 ?EventIsSystemEvent@CMainGameComponent@@IAE_NPBVCGameEvent@@@Z (protected); callers
//   AddGameEvent 0x004164B4 and GetInputs 0x00416FDE bind this spelling. Was the stand-in ?IsSystemEvent@T@@QAE_NPBUEvent@@@Z.
// Layout: engine/CMainGameComponent.h (FABLE_RETAIL_CMAINGAMECOMPONENT_EVENTISSYSTEMEVENT), engine/CGameEvent.h (Type +0x00).
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_EVENTISSYSTEMEVENT
#include "engine/CGameEvent.h"
#include "engine/CMainGameComponent.h"

bool CMainGameComponent::EventIsSystemEvent(const CGameEvent* event)
{
    switch (event->Type) {
    case 2:
        return true;
    }
    return false;
}
