// NCreatureMode::CModeExplosiveForcePushed::GetAnimFilter() const -- the mode's animation filter, the CAnimFilterOfGroup member Filter
// Leaf: ?GetAnimFilter@CModeExplosiveForcePushed@NCreatureMode@@UBEABVCAnimFilterBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008680F0, thiscall, `lea eax,[ecx+1Ch] ; ret`.
// Identity: slot 4 of NCreatureMode::CModeExplosiveForcePushed's vtable 0x012770B4; the Xbox 360 Test PDB virtual order (third source) aligned on 6 TLC-named slots puts GetAnimFilter() const at this slot; Ego_r/FableWin name set: ?GetAnimFilter@CModeExplosiveForcePushed@NCreatureMode@@UBEABVCAnimFilterBase@@XZ.
// Layout: Ego_r.pdb CModeExplosiveForcePushed has CAnimFilterOfGroup Filter at +0x1C (the offset retail reads); the bytes before it are the base class and earlier members (no shared header).
#include "engine/CAnimFilterOfGroup.h"
namespace NCreatureMode {
class CModeExplosiveForcePushed {
public:
    virtual const CAnimFilterBase& GetAnimFilter() const;
private:
    unsigned char _base_0x04[0x18];   // +0x04 base class and earlier members
    CAnimFilterOfGroup Filter;   // +0x1C
};
const CAnimFilterBase& CModeExplosiveForcePushed::GetAnimFilter() const
{
    return Filter;
}
}
