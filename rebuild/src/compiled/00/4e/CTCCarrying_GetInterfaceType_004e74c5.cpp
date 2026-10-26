// CTCCarrying::GetInterfaceType() const -- the component's interface type: 70 (0x46)
// Leaf: ?GetInterfaceType@CTCCarrying@@UBE?AW4ETCInterfaceType@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E74C5, 4 bytes (`push 46h; pop eax; ret`).
// Identity (sweep3 GetInterfaceType family; was "CTCCarrying::vfunc_21"): slot 21 of the CTCCarrying vtable 0x012433F4, directly before
//   GetClassTypeName (slot 22, named in 327 TC vtables): CTCBase declares GetInterfaceType then GetClassTypeName (Ego_r
//   0x007B0610 / 0x007B0620 for CTCAssassinRush); every one of the 244 slot-21 bodies returns a per-class constant
//   (CTCBoss = 0xAD agrees with the landed HasTC(0xad) / GetTC<CTCBoss>(0xad) callers). Ego_r/FableWin ?GetInterfaceType@CTCCarrying@@UBE?AW4ETCInterfaceType@@XZ.
// The ETCInterfaceType enumerator names are not in the name sets, so the value is written as a cast.
#pragma optimize("s", on)                                    // the TU is size-optimised (push/pop for the constant), as landed
enum ETCInterfaceType { };

class CTCCarrying {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};

ETCInterfaceType CTCCarrying::GetInterfaceType() const
{
    return (ETCInterfaceType)0x46;
}
