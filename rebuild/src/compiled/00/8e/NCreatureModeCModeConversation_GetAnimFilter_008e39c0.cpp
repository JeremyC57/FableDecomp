// NCreatureMode::CModeConversation::GetAnimFilter() const -- the mode's animation filter, the CAnimFilterOfGroup member Filter
// Leaf: ?GetAnimFilter@CModeConversation@NCreatureMode@@UBEABVCAnimFilterBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E39C0, thiscall, `lea eax,[ecx+24h] ; ret`.
// Identity: slot 4 of NCreatureMode::CModeConversation's vtable 0x01285990; the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots puts GetAnimFilter() const at this slot; Ego_r/FableWin name set: ?GetAnimFilter@CModeConversation@NCreatureMode@@UBEABVCAnimFilterBase@@XZ.
// Layout: Ego_r.pdb CModeConversation has CAnimFilterOfGroup Filter at +0x24 (the offset retail reads); the bytes before it are the base class and earlier members (no shared header).
#include "engine/CAnimFilterOfGroup.h"
namespace NCreatureMode {
class CModeConversation {
public:
    virtual const CAnimFilterBase& GetAnimFilter() const;
private:
    unsigned char _base_0x04[0x20];   // +0x04 base class and earlier members
    CAnimFilterOfGroup Filter;   // +0x24
};
const CAnimFilterBase& CModeConversation::GetAnimFilter() const
{
    return Filter;
}
}
