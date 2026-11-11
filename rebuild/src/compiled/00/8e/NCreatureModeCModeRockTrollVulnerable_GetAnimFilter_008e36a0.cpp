// NCreatureMode::CModeRockTrollVulnerable::GetAnimFilter() const -- the mode's animation filter, the CAnimFilterOfGroup member Filter
// Leaf: ?GetAnimFilter@CModeRockTrollVulnerable@NCreatureMode@@UBEABVCAnimFilterBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E36A0, thiscall, `lea eax,[ecx+20h] ; ret`.
// Identity: slot 4 of NCreatureMode::CModeRockTrollVulnerable's vtable 0x01285774; the Xbox 360 Test PDB virtual order (third source) aligned on 6 TLC-named slots puts GetAnimFilter() const at this slot; Ego_r/FableWin name set: ?GetAnimFilter@CModeRockTrollVulnerable@NCreatureMode@@UBEABVCAnimFilterBase@@XZ.
// Layout: Ego_r.pdb CModeRockTrollVulnerable has CAnimFilterOfGroup Filter at +0x20 (the offset retail reads); the bytes before it are the base class and earlier members (no shared header).
#include "engine/CAnimFilterOfGroup.h"
namespace NCreatureMode {
class CModeRockTrollVulnerable {
public:
    virtual const CAnimFilterBase& GetAnimFilter() const;
private:
    unsigned char _base_0x04[0x1C];   // +0x04 base class and earlier members
    CAnimFilterOfGroup Filter;   // +0x20
};
const CAnimFilterBase& CModeRockTrollVulnerable::GetAnimFilter() const
{
    return Filter;
}
}
