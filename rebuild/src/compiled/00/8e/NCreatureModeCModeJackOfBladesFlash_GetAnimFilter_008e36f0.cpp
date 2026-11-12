// NCreatureMode::CModeJackOfBladesFlash::GetAnimFilter() const -- the mode's animation filter, the CAnimFilterOfGroup member Filter
// Leaf: ?GetAnimFilter@CModeJackOfBladesFlash@NCreatureMode@@UBEABVCAnimFilterBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E36F0, thiscall, `lea eax,[ecx+70h] ; ret`.
// Identity: slot 4 of NCreatureMode::CModeJackOfBladesFlash's vtable 0x012857A8; the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots puts GetAnimFilter() const at this slot; Ego_r/FableWin name set: ?GetAnimFilter@CModeJackOfBladesFlash@NCreatureMode@@UBEABVCAnimFilterBase@@XZ.
// Layout: Ego_r.pdb CModeJackOfBladesFlash has CAnimFilterOfGroup Filter at +0x70 (the offset retail reads); the bytes before it are the base class and earlier members (no shared header).
#include "engine/CAnimFilterOfGroup.h"
namespace NCreatureMode {
class CModeJackOfBladesFlash {
public:
    virtual const CAnimFilterBase& GetAnimFilter() const;
private:
    unsigned char _base_0x04[0x6C];   // +0x04 base class and earlier members
    CAnimFilterOfGroup Filter;   // +0x70
};
const CAnimFilterBase& CModeJackOfBladesFlash::GetAnimFilter() const
{
    return Filter;
}
}
