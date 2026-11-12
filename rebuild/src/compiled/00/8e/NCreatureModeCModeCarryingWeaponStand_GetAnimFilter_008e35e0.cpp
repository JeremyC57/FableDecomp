// NCreatureMode::CModeCarryingWeaponStand::GetAnimFilter() const -- the mode's animation filter, the CAnimFilterOfGroup member Filter
// Leaf: ?GetAnimFilter@CModeCarryingWeaponStand@NCreatureMode@@UBEABVCAnimFilterBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E35E0, thiscall, `lea eax,[ecx+68h] ; ret`.
// Identity: slot 4 of NCreatureMode::CModeCarryingWeaponStand's vtable 0x012856E4; the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots puts GetAnimFilter() const at this slot; Ego_r/FableWin name set: ?GetAnimFilter@CModeCarryingWeaponStand@NCreatureMode@@UBEABVCAnimFilterBase@@XZ.
// Layout: Ego_r.pdb CModeCarryingWeaponStand has CAnimFilterOfGroup Filter at +0x68 (the offset retail reads); the bytes before it are the base class and earlier members (no shared header).
#include "engine/CAnimFilterOfGroup.h"
namespace NCreatureMode {
class CModeCarryingWeaponStand {
public:
    virtual const CAnimFilterBase& GetAnimFilter() const;
private:
    unsigned char _base_0x04[0x64];   // +0x04 base class and earlier members
    CAnimFilterOfGroup Filter;   // +0x68
};
const CAnimFilterBase& CModeCarryingWeaponStand::GetAnimFilter() const
{
    return Filter;
}
}
