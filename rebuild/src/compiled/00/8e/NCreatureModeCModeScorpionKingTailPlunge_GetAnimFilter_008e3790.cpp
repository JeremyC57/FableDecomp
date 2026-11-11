// NCreatureMode::CModeScorpionKingTailPlunge::GetAnimFilter() const -- the mode's animation filter, the CAnimFilterOfGroup member Filter
// Leaf: ?GetAnimFilter@CModeScorpionKingTailPlunge@NCreatureMode@@UBEABVCAnimFilterBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E3790, thiscall, `lea eax,[ecx+78h] ; ret`.
// Identity: slot 4 of NCreatureMode::CModeScorpionKingTailPlunge's vtable 0x01285860; the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots puts GetAnimFilter() const at this slot; Ego_r/FableWin name set: ?GetAnimFilter@CModeScorpionKingTailPlunge@NCreatureMode@@UBEABVCAnimFilterBase@@XZ.
// Layout: Ego_r.pdb CModeScorpionKingTailPlunge has CAnimFilterOfGroup Filter at +0x78 (the offset retail reads); the bytes before it are the base class and earlier members (no shared header).
#include "engine/CAnimFilterOfGroup.h"
namespace NCreatureMode {
class CModeScorpionKingTailPlunge {
public:
    virtual const CAnimFilterBase& GetAnimFilter() const;
private:
    unsigned char _base_0x04[0x74];   // +0x04 base class and earlier members
    CAnimFilterOfGroup Filter;   // +0x78
};
const CAnimFilterBase& CModeScorpionKingTailPlunge::GetAnimFilter() const
{
    return Filter;
}
}
