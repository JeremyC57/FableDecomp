// CTCCreatureHive::GetInterfaceType() const -- the component's interface type: 98 (0x62)
// Leaf: ?GetInterfaceType@CTCCreatureHive@@UBE?AW4ETCInterfaceType@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D45E3, 4 bytes (`push 62h; pop eax; ret`).
// Identity (sweep3 GetInterfaceType family; was "CTCCreatureHive::vfunc_21"): slot 21 of the CTCCreatureHive vtable 0x0123BDCC, directly before
//   GetClassTypeName (slot 22, named in 327 TC vtables): CTCBase declares GetInterfaceType then GetClassTypeName (Ego_r
//   0x007B0610 / 0x007B0620 for CTCAssassinRush); every one of the 244 slot-21 bodies returns a per-class constant
//   (CTCBoss = 0xAD agrees with the landed HasTC(0xad) / GetTC<CTCBoss>(0xad) callers). Ego_r/FableWin ?GetInterfaceType@CTCCreatureHive@@UBE?AW4ETCInterfaceType@@XZ.
// The ETCInterfaceType enumerator names are not in the name sets, so the value is written as a cast.
#pragma optimize("s", on)                                    // the TU is size-optimised (push/pop for the constant), as landed
enum ETCInterfaceType { };

class CTCCreatureHive {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};

ETCInterfaceType CTCCreatureHive::GetInterfaceType() const
{
    return (ETCInterfaceType)0x62;
}
