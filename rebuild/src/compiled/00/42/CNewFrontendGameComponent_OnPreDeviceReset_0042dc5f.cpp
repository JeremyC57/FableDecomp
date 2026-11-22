// CNewFrontendGameComponent::OnPreDeviceReset() -- nothing to release before a device reset: an empty override (`ret`)
// Leaf: ?OnPreDeviceReset@CNewFrontendGameComponent@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042DC5F, 1 byte (`ret`); vtable 0x01230C94 slot 0 (the CDeviceResetCallback override).
// Names (NF233, 2026-11-22, q12638): Ego_r.pdb + FableWin.pdb + Ego_d.pdb ?OnPreDeviceReset@CNewFrontendGameComponent@@UAEXXZ (public virtual),
//   the spelling the ported ctor / dtor vtables bind in slot 0; was a row-local stand-in struct (QAEXXZ). Ported onto
//   engine/CNewFrontendGameComponent.h (the NF233 CTOR_DTOR opt-in declares the override).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_RUN
#define FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_FILENAMES_FILETIMES
#define FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_CTOR_DTOR
#include "engine/CNewFrontendGameComponent.h"

void CNewFrontendGameComponent::OnPreDeviceReset()
{
}
