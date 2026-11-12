// NCreatureMode::CModeIceTrollGroundSmashIcicleAssault::GetAnimFilter() const -- the mode's animation filter, the CAnimFilterOfGroup member Filter
// Leaf: ?GetAnimFilter@CModeIceTrollGroundSmashIcicleAssault@NCreatureMode@@UBEABVCAnimFilterBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00743CA0, thiscall, `lea eax,[ecx+70h] ; ret`.
// Identity: slot 4 of NCreatureMode::CModeIceTrollGroundSmashIcicleAssault's vtable 0x01268774; the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots puts GetAnimFilter() const at this slot; Ego_r/FableWin name set: ?GetAnimFilter@CModeIceTrollGroundSmashIcicleAssault@NCreatureMode@@UBEABVCAnimFilterBase@@XZ.
// Layout: Ego_r.pdb CModeIceTrollGroundSmashIcicleAssault has CAnimFilterOfGroup Filter at +0x70 (the offset retail reads); the bytes before it are the base class and earlier members (no shared header).
#include "engine/CAnimFilterOfGroup.h"
namespace NCreatureMode {
class CModeIceTrollGroundSmashIcicleAssault {
public:
    virtual const CAnimFilterBase& GetAnimFilter() const;
private:
    unsigned char _base_0x04[0x6C];   // +0x04 base class and earlier members
    CAnimFilterOfGroup Filter;   // +0x70
};
const CAnimFilterBase& CModeIceTrollGroundSmashIcicleAssault::GetAnimFilter() const
{
    return Filter;
}
}
