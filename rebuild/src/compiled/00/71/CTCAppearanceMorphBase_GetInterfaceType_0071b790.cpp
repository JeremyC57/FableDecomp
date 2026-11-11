// CTCAppearanceMorphBase::GetInterfaceType() const -- returns TCI_APPEARANCE_MORPH
// Leaf: ?GetInterfaceType@CTCAppearanceMorphBase@@UBE?AW4ETCInterfaceType@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0071B790, thiscall, `ret`.
// Identity: slot 21 of CTCRandomAppearanceMorph's vtable 0x0126653C (defined here: the slot holds this body in 4 vtable(s), all of CTCAppearanceMorphBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 14 TLC-named slots of that vtable puts GetInterfaceType(void) at this slot; Ego_r/FableWin name set: ?GetInterfaceType@CTCAppearanceMorphBase@@UBE?AW4ETCInterfaceType@@XZ; `ret` matches the signature.
#include "engine/ETCInterfaceType.h"
class CTCAppearanceMorphBase {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCAppearanceMorphBase::GetInterfaceType() const
{
    return TCI_APPEARANCE_MORPH;
}
