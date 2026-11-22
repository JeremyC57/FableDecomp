// CNewFrontendGameComponent::OnPostDeviceReset() -- nothing to rebuild after a device reset: report success (`mov al,1 ; ret`)
// Leaf: ?OnPostDeviceReset@CNewFrontendGameComponent@@UAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042DC60, 3 bytes (`mov al,1 ; ret`); vtable 0x01230C94 slot 1 (the CDeviceResetCallback override).
// Names (NF233, 2026-11-22, q12638): Ego_r.pdb + FableWin.pdb + Ego_d.pdb ?OnPostDeviceReset@CNewFrontendGameComponent@@UAE_NXZ (public virtual),
//   the spelling the ported ctor / dtor vtables bind in slot 1; was a row-local stand-in struct (QAE_NXZ). Ported onto
//   engine/CNewFrontendGameComponent.h (the NF233 CTOR_DTOR opt-in declares the override).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_RUN
#define FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_FILENAMES_FILETIMES
#define FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_CTOR_DTOR
#include "engine/CNewFrontendGameComponent.h"

bool CNewFrontendGameComponent::OnPostDeviceReset()
{
    return true;
}
