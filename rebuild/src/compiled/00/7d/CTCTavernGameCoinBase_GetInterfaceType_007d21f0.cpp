// CTCTavernGameCoinBase::GetInterfaceType() const -- returns TCI_TAVERN_GAME
// Leaf: ?GetInterfaceType@CTCTavernGameCoinBase@@UBE?AW4ETCInterfaceType@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007D21F0, thiscall, `ret`.
// Identity: slot 21 of CTCTavernGameCoinGolf's vtable 0x0126F49C (defined here: the slot holds this body in 3 vtable(s), all of CTCTavernGameCoinBase or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 31 TLC-named slots of that vtable puts GetInterfaceType(void) at this slot; Ego_r/FableWin name set: ?GetInterfaceType@CTCTavernGameCoinBase@@UBE?AW4ETCInterfaceType@@XZ; `ret` matches the signature.
#include "engine/ETCInterfaceType.h"
class CTCTavernGameCoinBase {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
ETCInterfaceType CTCTavernGameCoinBase::GetInterfaceType() const
{
    return TCI_TAVERN_GAME;
}
