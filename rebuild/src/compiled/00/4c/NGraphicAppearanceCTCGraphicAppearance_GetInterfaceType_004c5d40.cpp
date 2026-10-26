// NGraphicAppearance::CTCGraphicAppearance::GetInterfaceType() const -- the component's interface type: 91 (0x5B)
// Leaf: ?GetInterfaceType@CTCGraphicAppearance@NGraphicAppearance@@UBE?AW4ETCInterfaceType@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004C5D40, 6 bytes (`mov eax,5Bh; ret`).
// Identity (sweep3 GetInterfaceType family; was "NGraphicAppearance::CTCGraphicAppearance::vfunc_21"): slot 21 of the NGraphicAppearance::CTCGraphicAppearance vtable 0x0123986C, directly before
//   GetClassTypeName (slot 22, named in 327 TC vtables): CTCBase declares GetInterfaceType then GetClassTypeName (Ego_r
//   0x007B0610 / 0x007B0620 for CTCAssassinRush); every one of the 244 slot-21 bodies returns a per-class constant
//   (CTCBoss = 0xAD agrees with the landed HasTC(0xad) / GetTC<CTCBoss>(0xad) callers). Ego_r/FableWin ?GetInterfaceType@CTCGraphicAppearance@NGraphicAppearance@@UBE?AW4ETCInterfaceType@@XZ.
// The ETCInterfaceType enumerator names are not in the name sets, so the value is written as a cast.
enum ETCInterfaceType { };

namespace NGraphicAppearance {
class CTCGraphicAppearance {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};
}

ETCInterfaceType NGraphicAppearance::CTCGraphicAppearance::GetInterfaceType() const
{
    return (ETCInterfaceType)0x5B;
}
