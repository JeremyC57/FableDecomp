#pragma optimize("s",on)
// CMainGameComponent::ProcessEvent(const CGameEvent* event) -- dispatch a system event: event type 2 calls the component's own SetQuit (vtable slot 4)
// Leaf: ?ProcessEvent@CMainGameComponent@@IAEXPBVCGameEvent@@@Z
// Retail: Fable.exe @ 0x00415FE0, 18 bytes, `ret 4`; the single-case switch lowers to `dec eax ; dec eax ; jne` (VC7.1).
// Name: Ego_r.pdb 0x00415194 / FableWin.pdb 0x018A872E ?ProcessEvent@CMainGameComponent@@IAEXPBVCGameEvent@@@Z (protected); callers
//   GetInputs 0x00416FED and ProcessEventPackage 0x00416691 bind this spelling. Was the stand-in ?ProcessEvent@T@@QAEXPAUEvent@@@Z.
// Layout: engine/CMainGameComponent.h (FABLE_RETAIL_CMAINGAMECOMPONENT_PROCESSEVENT, NF280; SetQuit is CGameComponent's slot 4 virtual,
//   `call [eax+10h]`), engine/CGameEvent.h (Type +0x00).
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PROCESSEVENT
#include "engine/CMainGameComponent.h"
#include "engine/CGameEvent.h"

void CMainGameComponent::ProcessEvent(const CGameEvent* event)
{
    switch (event->Type) {
    case 2:
        SetQuit();
        break;
    }
}
