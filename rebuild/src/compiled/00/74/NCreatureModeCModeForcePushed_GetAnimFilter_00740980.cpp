// NCreatureMode::CModeForcePushed::GetAnimFilter() const -- the mode's animation filter, the CAnimFilterOfGroup member Filter
// Leaf: ?GetAnimFilter@CModeForcePushed@NCreatureMode@@UBEABVCAnimFilterBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00740980, thiscall, `lea eax,[ecx+1Ch] ; ret`.
// Identity: slot 4 of NCreatureMode::CModeForcePushed's vtable 0x01267EBC; the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots puts GetAnimFilter() const at this slot; Ego_r/FableWin name set: ?GetAnimFilter@CModeForcePushed@NCreatureMode@@UBEABVCAnimFilterBase@@XZ.
// Layout: Ego_r.pdb CModeForcePushed has CAnimFilterOfGroup Filter at +0x1C (the offset retail reads); the bytes before it are the base class and earlier members (no shared header).
#include "engine/CAnimFilterOfGroup.h"
namespace NCreatureMode {
class CModeForcePushed {
public:
    virtual const CAnimFilterBase& GetAnimFilter() const;
private:
    unsigned char _base_0x04[0x18];   // +0x04 base class and earlier members
    CAnimFilterOfGroup Filter;   // +0x1C
};
const CAnimFilterBase& CModeForcePushed::GetAnimFilter() const
{
    return Filter;
}
}
