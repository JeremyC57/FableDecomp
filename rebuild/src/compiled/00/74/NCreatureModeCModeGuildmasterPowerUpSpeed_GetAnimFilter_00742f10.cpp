// NCreatureMode::CModeGuildmasterPowerUpSpeed::GetAnimFilter() const -- the mode's animation filter, the CAnimFilterOfGroup member Filter
// Leaf: ?GetAnimFilter@CModeGuildmasterPowerUpSpeed@NCreatureMode@@UBEABVCAnimFilterBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00742F10, thiscall, `lea eax,[ecx+68h] ; ret`.
// Identity: slot 4 of NCreatureMode::CModeGuildmasterPowerUpSpeed's vtable 0x01268624; the Xbox 360 Test PDB virtual order (third source) aligned on 6 TLC-named slots puts GetAnimFilter() const at this slot; Ego_r/FableWin name set: ?GetAnimFilter@CModeGuildmasterPowerUpSpeed@NCreatureMode@@UBEABVCAnimFilterBase@@XZ.
// Layout: Ego_r.pdb CModeGuildmasterPowerUpSpeed has CAnimFilterOfGroup Filter at +0x68 (the offset retail reads); the bytes before it are the base class and earlier members (no shared header).
#include "engine/CAnimFilterOfGroup.h"
namespace NCreatureMode {
class CModeGuildmasterPowerUpSpeed {
public:
    virtual const CAnimFilterBase& GetAnimFilter() const;
private:
    unsigned char _base_0x04[0x64];   // +0x04 base class and earlier members
    CAnimFilterOfGroup Filter;   // +0x68
};
const CAnimFilterBase& CModeGuildmasterPowerUpSpeed::GetAnimFilter() const
{
    return Filter;
}
}
