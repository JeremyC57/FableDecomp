// NCreatureMode::CModeJackOfBladesScream::GetAnimFilter() const -- the mode's animation filter, the CAnimFilterOfGroup member Filter
// Leaf: ?GetAnimFilter@CModeJackOfBladesScream@NCreatureMode@@UBEABVCAnimFilterBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00740510, thiscall, `lea eax,[ecx+68h] ; ret`.
// Identity: slot 4 of NCreatureMode::CModeJackOfBladesScream's vtable 0x01267D5C; the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots puts GetAnimFilter() const at this slot; Ego_r/FableWin name set: ?GetAnimFilter@CModeJackOfBladesScream@NCreatureMode@@UBEABVCAnimFilterBase@@XZ.
// Layout: Ego_r.pdb CModeJackOfBladesScream has CAnimFilterOfGroup Filter at +0x68 (the offset retail reads); the bytes before it are the base class and earlier members (no shared header).
#include "engine/CAnimFilterOfGroup.h"
namespace NCreatureMode {
class CModeJackOfBladesScream {
public:
    virtual const CAnimFilterBase& GetAnimFilter() const;
private:
    unsigned char _base_0x04[0x64];   // +0x04 base class and earlier members
    CAnimFilterOfGroup Filter;   // +0x68
};
const CAnimFilterBase& CModeJackOfBladesScream::GetAnimFilter() const
{
    return Filter;
}
}
