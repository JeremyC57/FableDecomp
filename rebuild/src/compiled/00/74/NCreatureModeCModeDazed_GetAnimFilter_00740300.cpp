// NCreatureMode::CModeDazed::GetAnimFilter() const -- the mode's animation filter, the CAnimFilterOfGroup member Filter
// Leaf: ?GetAnimFilter@CModeDazed@NCreatureMode@@UBEABVCAnimFilterBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00740300, thiscall, `lea eax,[ecx+78h] ; ret`.
// Identity: slot 4 of NCreatureMode::CModeDazed's vtable 0x01267CC4; the Xbox 360 Test PDB virtual order (third source) aligned on 8 TLC-named slots puts GetAnimFilter() const at this slot; Ego_r/FableWin name set: ?GetAnimFilter@CModeDazed@NCreatureMode@@UBEABVCAnimFilterBase@@XZ.
// Layout: Ego_r.pdb CModeDazed has CAnimFilterOfGroup Filter at +0x78 (the offset retail reads); the bytes before it are the base class and earlier members (no shared header).
#include "engine/CAnimFilterOfGroup.h"
namespace NCreatureMode {
class CModeDazed {
public:
    virtual const CAnimFilterBase& GetAnimFilter() const;
private:
    unsigned char _base_0x04[0x74];   // +0x04 base class and earlier members
    CAnimFilterOfGroup Filter;   // +0x78
};
const CAnimFilterBase& CModeDazed::GetAnimFilter() const
{
    return Filter;
}
}
