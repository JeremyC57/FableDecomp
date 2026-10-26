// CTCMultiStrike::GetInterfaceType() const -- the component's interface type: 135 (0x87)
// Leaf: ?GetInterfaceType@CTCMultiStrike@@UBE?AW4ETCInterfaceType@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007CD7D0, 6 bytes (`mov eax,87h; ret`).
// Identity (sweep3 GetInterfaceType family; was "CTCMultiStrike::vfunc_21"): slot 21 of the CTCMultiStrike vtable 0x0126EE0C, directly before
//   GetClassTypeName (slot 22, named in 327 TC vtables): CTCBase declares GetInterfaceType then GetClassTypeName (Ego_r
//   0x007B0610 / 0x007B0620 for CTCAssassinRush); every one of the 244 slot-21 bodies returns a per-class constant
//   (CTCBoss = 0xAD agrees with the landed HasTC(0xad) / GetTC<CTCBoss>(0xad) callers). Ego_r/FableWin ?GetInterfaceType@CTCMultiStrike@@UBE?AW4ETCInterfaceType@@XZ.
// The ETCInterfaceType enumerator names are not in the name sets, so the value is written as a cast.
enum ETCInterfaceType { };

class CTCMultiStrike {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};

ETCInterfaceType CTCMultiStrike::GetInterfaceType() const
{
    return (ETCInterfaceType)0x87;
}
