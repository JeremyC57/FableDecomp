// CTCHasName::GetInterfaceType() const -- the component's interface type: 268 (0x10C)
// Leaf: ?GetInterfaceType@CTCHasName@@UBE?AW4ETCInterfaceType@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DB55F, 6 bytes (`mov eax,10Ch; ret`).
// Identity (sweep3 GetInterfaceType family; was "CTCHasName::vfunc_21"): slot 21 of the CTCHasName vtable 0x01240AA4, directly before
//   GetClassTypeName (slot 22, named in 327 TC vtables): CTCBase declares GetInterfaceType then GetClassTypeName (Ego_r
//   0x007B0610 / 0x007B0620 for CTCAssassinRush); every one of the 244 slot-21 bodies returns a per-class constant
//   (CTCBoss = 0xAD agrees with the landed HasTC(0xad) / GetTC<CTCBoss>(0xad) callers). Ego_r/FableWin ?GetInterfaceType@CTCHasName@@UBE?AW4ETCInterfaceType@@XZ.
// The ETCInterfaceType enumerator names are not in the name sets, so the value is written as a cast.
enum ETCInterfaceType { };

class CTCHasName {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};

ETCInterfaceType CTCHasName::GetInterfaceType() const
{
    return (ETCInterfaceType)0x10C;
}
