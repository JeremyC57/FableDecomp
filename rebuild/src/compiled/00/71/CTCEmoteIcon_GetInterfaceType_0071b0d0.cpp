// CTCEmoteIcon::GetInterfaceType() const -- the component's interface type: 69 (0x45)
// Leaf: ?GetInterfaceType@CTCEmoteIcon@@UBE?AW4ETCInterfaceType@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0071B0D0, 6 bytes (`mov eax,45h; ret`).
// Identity (sweep3 GetInterfaceType family; was "CTCEmoteIcon::vfunc_21"): slot 21 of the CTCEmoteIcon vtable 0x01265ABC, directly before
//   GetClassTypeName (slot 22, named in 327 TC vtables): CTCBase declares GetInterfaceType then GetClassTypeName (Ego_r
//   0x007B0610 / 0x007B0620 for CTCAssassinRush); every one of the 244 slot-21 bodies returns a per-class constant
//   (CTCBoss = 0xAD agrees with the landed HasTC(0xad) / GetTC<CTCBoss>(0xad) callers). Ego_r/FableWin ?GetInterfaceType@CTCEmoteIcon@@UBE?AW4ETCInterfaceType@@XZ.
// The ETCInterfaceType enumerator names are not in the name sets, so the value is written as a cast.
enum ETCInterfaceType { };

class CTCEmoteIcon {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};

ETCInterfaceType CTCEmoteIcon::GetInterfaceType() const
{
    return (ETCInterfaceType)0x45;
}
