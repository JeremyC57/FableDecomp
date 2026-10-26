// CTCSearchableContainer::GetInterfaceType() const -- the component's interface type: 60 (0x3C)
// Leaf: ?GetInterfaceType@CTCSearchableContainer@@UBE?AW4ETCInterfaceType@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E48C3, 4 bytes (`push 3Ch; pop eax; ret`).
// Identity (sweep3 GetInterfaceType family; was "CTCSearchableContainer::vfunc_21"): slot 21 of the CTCSearchableContainer vtable 0x012431F4, directly before
//   GetClassTypeName (slot 22, named in 327 TC vtables): CTCBase declares GetInterfaceType then GetClassTypeName (Ego_r
//   0x007B0610 / 0x007B0620 for CTCAssassinRush); every one of the 244 slot-21 bodies returns a per-class constant
//   (CTCBoss = 0xAD agrees with the landed HasTC(0xad) / GetTC<CTCBoss>(0xad) callers). CTCSearchableContainer is not in the Ego_r/FableWin name sets (a class without a PDB symbol here), so the name rests on the slot and the body alone.
// The ETCInterfaceType enumerator names are not in the name sets, so the value is written as a cast.
#pragma optimize("s", on)                                    // the TU is size-optimised (push/pop for the constant), as landed
enum ETCInterfaceType { };

class CTCSearchableContainer {
public:
    virtual ETCInterfaceType GetInterfaceType() const;
};

ETCInterfaceType CTCSearchableContainer::GetInterfaceType() const
{
    return (ETCInterfaceType)0x3C;
}
