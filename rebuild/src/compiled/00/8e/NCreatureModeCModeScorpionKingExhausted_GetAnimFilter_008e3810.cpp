// NCreatureMode::CModeScorpionKingExhausted::GetAnimFilter() const -- the mode's animation filter, the CAnimFilterOfGroup member Filter
// Leaf: ?GetAnimFilter@CModeScorpionKingExhausted@NCreatureMode@@UBEABVCAnimFilterBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E3810, thiscall, `lea eax,[ecx+6Ch] ; ret`.
// Identity: slot 4 of NCreatureMode::CModeScorpionKingExhausted's vtable 0x012858A0; the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots puts GetAnimFilter() const at this slot; Ego_r/FableWin name set: ?GetAnimFilter@CModeScorpionKingExhausted@NCreatureMode@@UBEABVCAnimFilterBase@@XZ.
// Layout: Ego_r.pdb CModeScorpionKingExhausted has CAnimFilterOfGroup Filter at +0x6C (the offset retail reads); the bytes before it are the base class and earlier members (no shared header).
#include "engine/CAnimFilterOfGroup.h"
namespace NCreatureMode {
class CModeScorpionKingExhausted {
public:
    virtual const CAnimFilterBase& GetAnimFilter() const;
private:
    unsigned char _base_0x04[0x68];   // +0x04 base class and earlier members
    CAnimFilterOfGroup Filter;   // +0x6C
};
const CAnimFilterBase& CModeScorpionKingExhausted::GetAnimFilter() const
{
    return Filter;
}
}
