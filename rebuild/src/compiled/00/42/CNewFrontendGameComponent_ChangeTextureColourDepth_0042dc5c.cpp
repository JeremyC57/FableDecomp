// CNewFrontendGameComponent::ChangeTextureColourDepth(long depth) -- the front end keeps no textures to convert: an empty override (`ret 4`)
// Leaf: ?ChangeTextureColourDepth@CNewFrontendGameComponent@@UAEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0042DC5C, 3 bytes (`ret 4`); vtable 0x01230CA0 slot 3.
// Names (NF233, 2026-11-22, q12638): Ego_r.pdb + FableWin.pdb + Ego_d.pdb ?ChangeTextureColourDepth@CNewFrontendGameComponent@@UAEXJ@Z (public virtual,
//   long), the spelling the ported ctor / dtor vtables bind in slot 3; was a row-local stand-in struct (QAEXPBD, const char*). Ported onto
//   engine/CNewFrontendGameComponent.h (RUN declares the override).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_RUN
#define FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_FILENAMES_FILETIMES
#define FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_CTOR_DTOR
#include "engine/CNewFrontendGameComponent.h"

void CNewFrontendGameComponent::ChangeTextureColourDepth(long)
{
}
