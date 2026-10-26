// CTCDecapitation::GetInterfaceType() const -- the component's interface type: 209 (0xD1)
// Leaf: ?GetInterfaceType@CTCDecapitation@@UBE?AW4ETCInterfaceType@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DB227, 6 bytes (`mov eax,D1h; ret`).
// Identity (sweep3 GetInterfaceType family; was "CTCDecapitation::vfunc_21"): slot 21 of the CTCDecapitation vtable 0x01240534, directly before
//   GetClassTypeName (slot 22, named in 327 TC vtables): CTCBase declares GetInterfaceType then GetClassTypeName (Ego_r
//   0x007B0610 / 0x007B0620 for CTCAssassinRush); every one of the 244 slot-21 bodies returns a per-class constant
//   (CTCBoss = 0xAD agrees with the landed HasTC(0xad) / GetTC<CTCBoss>(0xad) callers). Ego_r/FableWin ?GetInterfaceType@CTCDecapitation@@UBE?AW4ETCInterfaceType@@XZ.
// The ETCInterfaceType enumerator names are not in the name sets, so the value is written as a cast.
enum ETCInterfaceType { };

class CTCDecapitation {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};

ETCInterfaceType CTCDecapitation::GetInterfaceType() const
{
    return (ETCInterfaceType)0xD1;
}
