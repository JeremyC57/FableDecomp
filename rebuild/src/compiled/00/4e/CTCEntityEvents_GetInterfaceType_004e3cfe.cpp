// CTCEntityEvents::GetInterfaceType() const -- the component's interface type: 66 (0x42)
// Leaf: ?GetInterfaceType@CTCEntityEvents@@UBE?AW4ETCInterfaceType@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E3CFE, 4 bytes (`push 42h; pop eax; ret`).
// Identity (sweep3 GetInterfaceType family; was "CTCEntityEvents::vfunc_21"): slot 21 of the CTCEntityEvents vtable 0x01242FDC, directly before
//   GetClassTypeName (slot 22, named in 327 TC vtables): CTCBase declares GetInterfaceType then GetClassTypeName (Ego_r
//   0x007B0610 / 0x007B0620 for CTCAssassinRush); every one of the 244 slot-21 bodies returns a per-class constant
//   (CTCBoss = 0xAD agrees with the landed HasTC(0xad) / GetTC<CTCBoss>(0xad) callers). Ego_r/FableWin ?GetInterfaceType@CTCEntityEvents@@UBE?AW4ETCInterfaceType@@XZ.
// The ETCInterfaceType enumerator names are not in the name sets, so the value is written as a cast.
#pragma optimize("s", on)                                    // the TU is size-optimised (push/pop for the constant), as landed
enum ETCInterfaceType { };

class CTCEntityEvents {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};

ETCInterfaceType CTCEntityEvents::GetInterfaceType() const
{
    return (ETCInterfaceType)0x42;
}
