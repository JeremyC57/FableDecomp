// CTCUnholyPower::GetInterfaceType() const -- the component's interface type: 134 (0x86)
// Leaf: ?GetInterfaceType@CTCUnholyPower@@UBE?AW4ETCInterfaceType@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00798150, 6 bytes (`mov eax,86h; ret`).
// Identity (sweep3 GetInterfaceType family; was "CTCUnholyPower::vfunc_21"): slot 21 of the CTCUnholyPower vtable 0x0126BFDC, directly before
//   GetClassTypeName (slot 22, named in 327 TC vtables): CTCBase declares GetInterfaceType then GetClassTypeName (Ego_r
//   0x007B0610 / 0x007B0620 for CTCAssassinRush); every one of the 244 slot-21 bodies returns a per-class constant
//   (CTCBoss = 0xAD agrees with the landed HasTC(0xad) / GetTC<CTCBoss>(0xad) callers). Ego_r/FableWin ?GetInterfaceType@CTCUnholyPower@@UBE?AW4ETCInterfaceType@@XZ.
// The ETCInterfaceType enumerator names are not in the name sets, so the value is written as a cast.
enum ETCInterfaceType { };

class CTCUnholyPower {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};

ETCInterfaceType CTCUnholyPower::GetInterfaceType() const
{
    return (ETCInterfaceType)0x86;
}
