// NCreatureMode::CModeDragonBase::GetAnimFilter() const -- the mode's animation filter, the CAnimFilterOfGroup member Filter
// Leaf: ?GetAnimFilter@CModeDragonBase@NCreatureMode@@UBEABVCAnimFilterBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00741D50, thiscall, `lea eax,[ecx+1Ch] ; ret`.
// Identity: slot 4 of NCreatureMode::CModeIntangible's vtable 0x01268514; the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots puts GetAnimFilter() const at this slot; Ego_r/FableWin name set: ?GetAnimFilter@CModeDragonBase@NCreatureMode@@UBEABVCAnimFilterBase@@XZ.
// Layout: Ego_r.pdb CModeDragonBase has CAnimFilterOfGroup Filter at +0x1C (the offset retail reads); the bytes before it are the base class and earlier members (engine/CModeDragonBase.h under FABLE_RETAIL_CMODEDRAGONBASE_VSLOTS).
#define FABLE_RETAIL_CMODEDRAGONBASE_VSLOTS
#include "engine/CModeDragonBase.h"
namespace NCreatureMode {
const CAnimFilterBase& CModeDragonBase::GetAnimFilter() const
{
    return Filter;
}
}
