// NCreatureMode::CModeScared::GetAnimFilter() const -- the mode's animation filter, the CAnimFilterOfGroup member Filter
// Leaf: ?GetAnimFilter@CModeScared@NCreatureMode@@UBEABVCAnimFilterBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0073EF90, thiscall, `lea eax,[ecx+68h] ; ret`.
// Identity: slot 4 of NCreatureMode::CModeScared's vtable 0x012676A4; the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots puts GetAnimFilter() const at this slot; Ego_r/FableWin name set: ?GetAnimFilter@CModeScared@NCreatureMode@@UBEABVCAnimFilterBase@@XZ.
// Layout: Ego_r.pdb CModeScared has CAnimFilterOfGroup Filter at +0x68 (the offset retail reads); the bytes before it are the base class and earlier members (no shared header).
#include "engine/CAnimFilterOfGroup.h"
namespace NCreatureMode {
class CModeScared {
public:
    virtual const CAnimFilterBase& GetAnimFilter() const;
private:
    unsigned char _base_0x04[0x64];   // +0x04 base class and earlier members
    CAnimFilterOfGroup Filter;   // +0x68
};
const CAnimFilterBase& CModeScared::GetAnimFilter() const
{
    return Filter;
}
}
