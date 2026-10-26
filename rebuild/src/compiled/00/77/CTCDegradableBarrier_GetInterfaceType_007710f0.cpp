// CTCDegradableBarrier::GetInterfaceType() const -- the component's interface type: 103 (0x67)
// Leaf: ?GetInterfaceType@CTCDegradableBarrier@@UBE?AW4ETCInterfaceType@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007710F0, 6 bytes (`mov eax,67h; ret`).
// Identity (sweep3 GetInterfaceType family; was "CTCDegradableBarrier::vfunc_21"): slot 21 of the CTCDegradableBarrier vtable 0x0126A6A4, directly before
//   GetClassTypeName (slot 22, named in 327 TC vtables): CTCBase declares GetInterfaceType then GetClassTypeName (Ego_r
//   0x007B0610 / 0x007B0620 for CTCAssassinRush); every one of the 244 slot-21 bodies returns a per-class constant
//   (CTCBoss = 0xAD agrees with the landed HasTC(0xad) / GetTC<CTCBoss>(0xad) callers). CTCDegradableBarrier is not in the Ego_r/FableWin name sets (a class without a PDB symbol here), so the name rests on the slot and the body alone.
// The ETCInterfaceType enumerator names are not in the name sets, so the value is written as a cast.
enum ETCInterfaceType { };

class CTCDegradableBarrier {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};

ETCInterfaceType CTCDegradableBarrier::GetInterfaceType() const
{
    return (ETCInterfaceType)0x67;
}
