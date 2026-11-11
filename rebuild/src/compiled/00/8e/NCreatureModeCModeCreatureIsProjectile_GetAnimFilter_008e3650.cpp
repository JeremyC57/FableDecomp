// NCreatureMode::CModeCreatureIsProjectile::GetAnimFilter() const -- the mode's animation filter, the CAnimFilterOfGroup member Filter
// Leaf: ?GetAnimFilter@CModeCreatureIsProjectile@NCreatureMode@@UBEABVCAnimFilterBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E3650, thiscall, `lea eax,[ecx+74h] ; ret`.
// Identity: slot 4 of NCreatureMode::CModeCreatureIsProjectile's vtable 0x01285728; the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots puts GetAnimFilter() const at this slot; Ego_r/FableWin name set: ?GetAnimFilter@CModeCreatureIsProjectile@NCreatureMode@@UBEABVCAnimFilterBase@@XZ.
// Layout: Ego_r.pdb CModeCreatureIsProjectile has CAnimFilterOfGroup Filter at +0x74 (the offset retail reads); the bytes before it are the base class and earlier members (no shared header).
#include "engine/CAnimFilterOfGroup.h"
namespace NCreatureMode {
class CModeCreatureIsProjectile {
public:
    virtual const CAnimFilterBase& GetAnimFilter() const;
private:
    unsigned char _base_0x04[0x70];   // +0x04 base class and earlier members
    CAnimFilterOfGroup Filter;   // +0x74
};
const CAnimFilterBase& CModeCreatureIsProjectile::GetAnimFilter() const
{
    return Filter;
}
}
