// NCreatureMode::CModeSitTavernStool::GetAnimFilter() const -- the mode's animation filter, the CAnimFilterOfGroup member Filter
// Leaf: ?GetAnimFilter@CModeSitTavernStool@NCreatureMode@@UBEABVCAnimFilterBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0094B5C0, thiscall, `lea eax,[ecx+2Ch] ; ret`.
// Identity: slot 4 of NCreatureMode::CModeSitTavernStool's vtable 0x01292FF8; the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots puts GetAnimFilter() const at this slot; Ego_r/FableWin name set: ?GetAnimFilter@CModeSitTavernStool@NCreatureMode@@UBEABVCAnimFilterBase@@XZ.
// Layout: Ego_r.pdb CModeSitTavernStool has CAnimFilterOfGroup Filter at +0x2C (the offset retail reads); the bytes before it are the base class and earlier members (no shared header).
#include "engine/CAnimFilterOfGroup.h"
namespace NCreatureMode {
class CModeSitTavernStool {
public:
    virtual const CAnimFilterBase& GetAnimFilter() const;
private:
    unsigned char _base_0x04[0x28];   // +0x04 base class and earlier members
    CAnimFilterOfGroup Filter;   // +0x2C
};
const CAnimFilterBase& CModeSitTavernStool::GetAnimFilter() const
{
    return Filter;
}
}
